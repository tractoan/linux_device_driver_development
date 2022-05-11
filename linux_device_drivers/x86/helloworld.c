/*
Softirqs as bottom half
As said in the beginning of this chapter, we will not discuss softirq. Tasklets will be enough
everywhere you feel the need to use softirqs. Anyway, let's talk about their defaults.
Softirqs run in a software interrupt context, with preemption disabled, holding the CPU
until they complete. Softirq should be fast; otherwise they may slow the system down.
When, for any reason, a softirq prevents the kernel from scheduling other tasks, any new
incoming softirq will be handled by ksoftirqd threads, running in a process context.
*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init helloworld_init(void)
{
	pr_info("Hello World!\n");
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