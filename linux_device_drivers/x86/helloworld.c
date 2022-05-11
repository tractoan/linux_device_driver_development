/*
The main goal of threaded IRQs is reducing the time spent with interrupts disabled to a
bare minimum. With threaded IRQs, the way you register an interrupt handler is a bit
simplified. You does not even have to schedule the bottom half yourself. The core does that
for us. The bottom half is then executed in a dedicated kernel thread. We do not use
request_irq() anymore, but request_threaded_irq()
	int request_threaded_irq(unsigned int irq, irq_handler_t handler,\
							irq_handler_t thread_fn, \
							unsigned long irqflags, \
							const char *devname, void *dev_id)
		@handler function: This is the same function as the one registered with
	request_irq(). It represents the top-half function, which runs in an atomic
	context (or hard-IRQ). If it can process the interrupt faster so that you can get rid
	of the bottom half at all, it should return IRQ_HANDLED. But, if the interrupt
	processing needs more than 100 μs, as discussed previously, you should use the
	bottom half. In this case, it should return IRQ_WAKE_THREAD, which will result in
	scheduling the thread_fn function that must have been provided.
		@thread_fn function: This represents the bottom half, as you would have
	scheduled in your top half. When the hard-IRQ handler (handler function)
	function returns IRQ_WAKE_THREAD, the kthread associated with this bottom half
	will be scheduled, invoking the thread_fn function when it comes to run the
	ktread. The thread_fn function must return IRQ_HANDLED when complete.
	After being executed, the kthread will not be rescheduled again until the IRQ is
	triggered again and the hard-IRQ returns IRQ_WAKE_THREAD
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
	return IRQ_WAKE_THREAD;
}

static irqreturn_t my_thread_irq_bottom_halves(int irq, void *dev_id)
{
	pr_info("This is thread irq bottom halves\n");
	return IRQ_HANDLED;
}

static int __init helloworld_init(void)
{
	int ret;
	pr_info("Hello World!\n");
	md = kzalloc(sizeof(*md), GFP_KERNEL);
	ret = request_threaded_irq(IRQ_NUMBER, my_irq_handler,
								my_thread_irq_bottom_halves,
								IRQF_SHARED,
								DRV_NAME, md);
	asm("int $0x3B");
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