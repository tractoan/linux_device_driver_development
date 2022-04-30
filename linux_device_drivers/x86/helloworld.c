#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include 

/*Software IRQ (softirq), or software interrupt is a deferring mechanism used only for very
fast processing, since it runs with a disabled scheduler (in an interrupt context). You'll
rarely (almost never) want to deal with softirq directly. There are only networks and block
device subsystems using softirq. Tasklets are an instantiation of softirqs, and will be
sufficient in almost every case that you feel the need to use softirqs*/

/*In most cases, softirqs are scheduled in hardware interrupts, which may arrive very quickly,
faster than they can be serviced. They are then queued by the kernel in order to be
processed later. Ksoftirqds are responsible for late execution (process context this time). A
ksoftirqd is a per-CPU kernel thread raised to handle unserviced software interrupts*/

/*When run "top", you can see ksoftirqd/n 
entries, where n is the CPU number that the ksoftirqd runs on. CPU-consuming ksoftirqd
may indicate an overloaded system or a system under interrupts storm, which is never
good. You can have a look at kernel/softirq.c to see how ksoftirqds are designed.
*/

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