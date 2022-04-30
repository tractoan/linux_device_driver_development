/*
	Added since Linux kernel 2.6, the most used and simple deferring mechanism is the work
queue. It is the last one we will talk about in this chapter. As a deferring mechanism, it takes
an opposite approach to the others we've seen, running only in a preemptible context. It is
the only choice when you need to sleep in your bottom half (I will explain what a bottom
half is later in the next section). By sleep, I mean process I/O data, hold mutexes, delay, and
all the other tasks that may lead to sleep or move the task off the run queue.
	Keep in mind that work queues are built on top of kernel threads, and this is the reason
why I decided not to talk about the kernel thread as a deferring mechanism at all. However,
there are two ways to deal with work queues in the kernel. First, there is a default shared
work queue, handled by a set of kernel threads, each running on a CPU. Once you have
work to schedule, you queue that work into the global work queue, which will be executed
at the appropriate moment. The other method is to run the work queue in a dedicated
kernel thread. It means whenever your work queue handler needs to be executed, your
kernel thread is woken up to handle it, instead of one of the default predefined threads.
	Structures and functions to call are different, depending on whether you chose a shared
work queue or dedicated ones
*/

/*
	Here, the work queue is represented as an instance of struct workqueue_struct. The
work to be queued into the work queue is represented as an instance of struct
work_struct. There are four steps involved prior to scheduling your work in your own
kernel thread:
	1. Declare/initialize a struct workqueue_struct.
	2. Create your work function.
	3. Create a struct work_struct so that your work function will be embedded3.
	into it.
	4. Embed your work function in the work_struct.
*/

/*The following functions are defined in include/linux/workqueue.h:
	Declare work and work queue:
		struct workqueue_struct *myqueue;
		struct work_struct thework;
		Define the worker function (the handler):
		void dowork(void *data) {  Code goes here  };
	Initialize our work queue and embed our work into:
		myqueue = create_singlethread_workqueue( "mywork" );
		INIT_WORK( &thework, dowork, <data-pointer> );
	We could have also created our work queues through a macro called
create_workqueue. The difference between create_workqueue and
create_singlethread_workqueue is that the former will create a work queue
that in turn will create a separate kernel thread on each and every processor
available.
	Scheduling work:
		queue_work(myqueue, &thework);
	Queue after the given delay to the given worker thread:
		queue_delayed_work(myqueue, &thework, <delay>);
	These functions return false if the work was already on a queue and true if
otherwise. delay represents the number of jiffies to wait before queueing. You
may use the helper function msecs_to_jiffies in order to convert the standard
ms delay into jiffies. For example, to queue a work after 5 ms, you can use
queue_delayed_work(myqueue, &thework, msecs_to_jiffies(5));.
	Wait on all pending work on the given work queue:
		void flush_workqueue(struct workqueue_struct *wq)
	flush_workqueue sleeps until all queued work has finished their execution. New
incoming (enqueued) work does not affect the sleep. One may typically use this in
driver shutdown handlers.
	Cleanup: Use cancel_work_sync() or cancel_delayed_work_sync for synchronous
cancellation, which will cancel the work if it is not already running, or block until
the work has completed. The work will be cancelled even if it requeues itself. You
must also ensure that the work queue on which the work was last queued can't be
destroyed before the handler returns. These functions are to be used respectively
for nondelayed or delayed work:
		int cancel_work_sync(struct work_struct *work);
		int cancel_delayed_work_sync(struct delayed_work *dwork);
	Since Linux kernel v4.8, it is possible to use cancel_work or cancel_delayed_work,
which are asynchronous forms of cancellation. One must check whether the function
returns true or no, and makes sure the work does not requeue itself. You must then
explicitly flush the work queue:
	if ( !cancel_delayed_work( &thework) ){
		flush_workqueue(myqueue);
		destroy_workqueue(myqueue);
	}
	The other is a different version of the same method and will create only a single thread for
all the processors. In case you need a delay before the work is enqueued, feel free to use the
following work initialization macro:
		INIT_DELAYED_WORK(_work, _func);
		INIT_DELAYED_WORK_DEFERRABLE(_work, _func);
Using the preceding macros would imply that you should use the following functions to
queue or schedule the work in the work queue:
		int queue_delayed_work(struct workqueue_struct *wq,
								struct delayed_work *dwork, unsigned long delay)
	queue_work ties the work to the current CPU. You can specify the CPU on which the
handler should run using the queue_work_on function:
		int queue_work_on(int cpu, struct workqueue_struct *wq,
								struct work_struct *work);
	For delayed work, you can use:
		int queue_delayed_work_on(int cpu, struct workqueue_struct *wq,
								struct delayed_work *dwork, unsigned long delay);
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/workqueue.h> /* for work queue */
#include <linux/slab.h> /* for kmalloc() */

struct workqueue_struct *wq;
struct work_data {
	struct work_struct my_work;
	int the_data;
};

static void work_handler(struct work_struct *work)
{
	struct work_data * my_data = container_of(work,
	struct work_data, my_work);
	printk("Work queue module handler: %s, data is %d\n",
	__FUNCTION__, my_data->the_data);
	kfree(my_data);
}

static int __init helloworld_init(void)
{
	pr_info("Hello World!\n");
	struct work_data * my_data;
	printk("Work queue module init: %s %d\n", __FUNCTION__, __LINE__);
	wq = create_singlethread_workqueue("my_single_thread");
	my_data = kmalloc(sizeof(struct work_data), GFP_KERNEL);
	my_data->the_data = 34;
	INIT_WORK(&my_data->my_work, work_handler);
	queue_work(wq, &my_data->my_work);
	return 0;
}

static void __exit helloworld_exit(void)
{
	flush_workqueue(wq);
	destroy_workqueue(wq);
	pr_info("End of the world!\n");
}

module_init(helloworld_init);
module_exit(helloworld_exit);

MODULE_AUTHOR("Toan Nguyen (toannguyenkt69@gmail.com)");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello World Module");
