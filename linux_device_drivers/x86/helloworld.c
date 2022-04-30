#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>
//A jiffy is a kernel unit of time declared in <linux/jiffies.h>. To understand jiffies, we need to introduce a new constant HZ, which is the number of times jiffies is incremented in one second. Each increment is called a tick.
#include <linux/timer.h>

//A timer is represented in the kernel as an instance of timer_list
static struct timer_list my_timer;

void my_timer_callback(struct timer_list *timer)
{
	int retval;
	printk("%s called (%ld).\n", __FUNCTION__, jiffies);
	retval = mod_timer( &my_timer, jiffies + HZ*2000/1000);
}

static int __init helloworld_init(void)
{
	int retval;
	pr_info("Hello World!\n");
	printk("Timer module loaded\n");
	timer_setup(&my_timer, my_timer_callback, 0);
	printk("Setup timer to fire in 2000ms (%ld)\n", jiffies);
	retval = mod_timer( &my_timer, jiffies + msecs_to_jiffies(2000));
	if (retval)
		printk("Timer firing failed\n");
	return 0;
}

static void __exit helloworld_exit(void)
{
	int retval;
	pr_info("End of the world!\n");
	retval = del_timer(&my_timer);
	/* Is timer still active (1) or no (0) */
	if (retval)
		printk("The timer is still in use...\n");
	pr_info("Timer module unloaded\n");
}

module_init(helloworld_init);
module_exit(helloworld_exit);

MODULE_AUTHOR("Toan Nguyen (toannguyenkt69@gmail.com)");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello World Module");
