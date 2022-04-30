#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/wait.h>
#include <linux/delay.h>

#ifdef STATIC_METHOD
DECLARE_WAIT_QUEUE_HEAD(my_wait_queue);
#else
wait_queue_head_t my_wait_queue;
#endif

static int condition = 0;

/* declare a work queue*/
static struct work_struct work;

static void work_handler(struct work_struct *work)
{
	printk("Waitqueue module handler %s\n", __FUNCTION__);
	msleep(5000);
	printk("Wake up the sleeping module\n");
	condition = 1;
	wake_up_interruptible(&my_wait_queue);
}

static int __init helloworld_init(void)
{
	pr_info("Hello World!\n");
#ifdef STATIC_METHOD

#else
	init_waitqueue_head(&my_wait_queue);
#endif

	INIT_WORK(&work, work_handler);
	schedule_work(&work);

	printk("Going to sleep %s\n", __FUNCTION__);
	wait_event_interruptible(my_wait_queue, condition != 0);
	// wait_event_interruptible does not continuously poll, but simply evaluates the condition when it is called. If the condition is false, the process is put into a TASK_INTERRUPTIBLE state and removed from the run queue. The condition is then only rechecked each time you call wake_up_interruptible in the wait queue. If the condition is true when wake_up_interruptible runs, a process in the wait queue will be awakened, and its state set to TASK_RUNNING. Processes are awakened in the order they are put to sleep. To awaken all processes waiting in the queue, you should use wake_up_interruptible_all
	pr_info("woken up by the work job\n");
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