/*
	You can register a callback to be run when the interruption (or interrupt line) you are
interested in gets fired. You can achieve that with the function request_irq(), declared in
<linux/interrupt.h>:
		int request_irq(unsigned int irq, irq_handler_t handler,
						unsigned long flags, const char *name, void *dev)
	request_irq() may fail, and return 0 on success. Other elements of the preceding code
are outlined in detail as follows:
	- flags: These should be a bitmask of the masks defined in <linux/interrupt.h>. The most used are:
		- IRQF_TIMER: Informs the kernel that this handler is originated by
		system timer interrupt.
		- IRQF_SHARED: Used for interrupt lines that can be shared by two
		or more devices. Each device sharing the same line must have this
		flag set. If omitted, only one handler can be registered for the
		specified IRQ line.
		- IRQF_ONESHOT: Used essentially in the threaded IRQ. It instructs
		the kernel not to re-enable the interrupt when the hardirq handler
		has finished. It will remain disabled until the threaded handler has
		been run.
		In older kernel versions (until v2.6.35), there were IRQF_DISABLED
		flags, which asked the kernel to disable all interrupts when the
		handler is running. This flag is no longer used.
	- name: This is used by the kernel to identify your driver in /proc/interrupts
	and /proc/irq.
	- dev: Its primary goal is to pass as argument to the handler. This should be unique
	to each registered handler, since it is used to identify the device. It can be NULL
	for nonshared IRQs, but not for shared ones. The common way of using it is to
	provide a device structure, since it is both unique and may potentially be useful
	to the handler. That said, a pointer to any per-device data structure is sufficient

*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h> /* for kmalloc() */
#include <linux/interrupt.h>

#define DRV_NAME "Hello World Driver"
#define IRQ_NUMBER 11

struct my_data {
	char name[64];
	char phys[32];
};

struct my_data *md;

static irqreturn_t my_irq_handler(int irq, void *dev_id)
{
	struct my_data *md = dev_id;
	pr_info("IRQ Handled\n");
	/* Check whether my device raised the irq or no */
	return IRQ_HANDLED;
}
/* some where in the code, in the probe function */

static int __init helloworld_init(void)
{
	int ret;

	pr_info("Hello World!\n");
	md = kzalloc(sizeof(*md), GFP_KERNEL);
	ret = request_irq(IRQ_NUMBER, my_irq_handler,
						IRQF_SHARED,
						DRV_NAME, md);
	asm("int $0x30");
	return 0;
}

static void __exit helloworld_exit(void)
{
	pr_info("End of the world!\n");
	free_irq(IRQ_NUMBER, md);
}

module_init(helloworld_init);
module_exit(helloworld_exit);

MODULE_AUTHOR("Toan Nguyen (toannguyenkt69@gmail.com)");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello World Module");