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
work queue or dedicated ones.
*/
/*
	Unless you have no choice, or you need critical performance, or you need to control
everything from the work queue initialization to the work scheduling, and if you only
submit tasks occasionally, you should use the shared work queue provided by the kernel.
With that queue being shared over the system, you should be nice, and should not
monopolize the queue for a long time.
	Since the execution of the pending task on the queue is serialized on each CPU, you should
not sleep for a long time because no other task on the queue will run until you wake up.
You won't even know who you share the work queue with, so don't be surprised if your
task takes longer to get the CPU. Work in the shared work queues is executed in a per-CPU
thread called events/n, created by the kernel.
	In this case, the work must also be initialized with the INIT_WORK macro. Since we are
going to use the shared work queue, there is no need to create a work queue structure. We
only need the work_struct structure that will be passed as an argument. There are three
functions to schedule work on the shared work queue:
	The version that ties the work on the current CPU:
		int schedule_work(struct work_struct *work);
	The same but delayed function:
		static inline bool schedule_delayed_work(struct delayed_work *dwork, unsigned long delay)
	The function that actually schedules the work on a given CPU:
		int schedule_work_on(int cpu, struct work_struct *work);
	The same as shown previously, but with a delay:
		int scheduled_delayed_work_on(int cpu, struct delayed_work *dwork, unsigned long delay);
*/
/*
	All of these functions schedule the work given as an argument on to the system's shared
work queue system_wq, defined in kernel/workqueue.c:
		struct workqueue_struct *system_wq __read_mostly;
		EXPORT_SYMBOL(system_wq);
	A work already submitted to the shared queue can be cancelled with the
cancel_delayed_work function. You can flush the shared workqueue with:
		void flush_scheduled_work(void);
	Since the queue is shared over the system, one can't really know how long
flush_scheduled_work() may last before it returns
*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h> /* for sleep */
#include <linux/wait.h> /* for wait queue */
#include <linux/time.h>
#include <linux/delay.h>
#include <linux/slab.h> /* for kmalloc() */
#include <linux/workqueue.h>

//static DECLARE_WAIT_QUEUE_HEAD(my_wq);
static int sleep = 0;
struct work_data {
	struct work_struct my_work;
	wait_queue_head_t my_wq;
	int the_data;
};

static void work_handler(struct work_struct *work)
{
	struct work_data *my_data;
	my_data = container_of(work, struct work_data, my_work);
	printk("Work queue module handler: %s, data is %d\n", __FUNCTION__, my_data->the_data);
	msleep(2000);
	wake_up_interruptible(&my_data->my_wq);
	sleep = 1;
}

static int __init helloworld_init(void)
{
	pr_info("Hello World!\n");
	struct work_data * my_data;
	my_data = kmalloc(sizeof(struct work_data), GFP_KERNEL);
	my_data->the_data = 34;
	INIT_WORK(&my_data->my_work, work_handler);
	init_waitqueue_head(&my_data->my_wq);
	schedule_work(&my_data->my_work);
	printk("I'm goint to sleep ...\n");
	wait_event_interruptible(my_data->my_wq, sleep != 0);
	printk("I am Waked up...\n");
	kfree(my_data);
	return 0;
}

static void __exit helloworld_exit(void)
{
	pr_info("End of the world!\n");
}

module_init(helloworld_init);
module_exit(helloworld_exit);

MODULE_AUTHOR("Toan Nguyen (toannguyenkt69@gmail.com)");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello World Module");
