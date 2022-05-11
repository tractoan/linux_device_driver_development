/*
Tasklet as bottom halves
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
	struct tasklet_struct my_tasklet;
};

struct my_data *md;

static irqreturn_t my_irq_handler(int irq, void *dev_id)
{
	struct my_data *md = dev_id;
	tasklet_schedule(&md->my_tasklet);
	pr_info("IRQ Handled\n");
	/* Check whether my device raised the irq or no */
	return IRQ_HANDLED;
}

void my_tasklet_bottom_halves(unsigned long data)
{
	pr_info("This is bottom halves\n");
}

/* some where in the code, in the probe function */
static int __init helloworld_init(void)
{
	int ret;

	pr_info("Hello World!\n");
	md = kzalloc(sizeof(*md), GFP_KERNEL);
	tasklet_init(&md->my_tasklet, my_tasklet_bottom_halves, (unsigned long)md);
	ret = request_irq(IRQ_NUMBER, my_irq_handler,
						IRQF_SHARED,
						DRV_NAME, md);
	asm("int $0x3B");
	return 0;
}

static void __exit helloworld_exit(void)
{
	pr_info("End of the world!\n");
	free_irq(IRQ_NUMBER, md);
	tasklet_kill(&md->my_tasklet);
}

module_init(helloworld_init);
module_exit(helloworld_exit);
MODULE_AUTHOR("Toan Nguyen (toannguyenkt69@gmail.com)");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello World Module");