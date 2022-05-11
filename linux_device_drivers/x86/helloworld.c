/*
Workqueue as bottom halves
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
	struct workqueue_struct *wq;
	struct work_struct my_work;
};

struct my_data *md;

static irqreturn_t my_irq_handler(int irq, void *dev_id)
{
	struct my_data *md = dev_id;
	schedule_work(&md->my_work);
	pr_info("IRQ Handled\n");
	/* Check whether my device raised the irq or no */
	return IRQ_HANDLED;
}

static void my_workqueue_bottom_halves(struct work_struct *work)
{
	struct my_data * data = container_of(work, struct my_data, my_work);
	pr_info("This is workqueue bottom halves\n");
}

/* some where in the code, in the probe function */

static int __init helloworld_init(void)
{
	int ret;

	pr_info("Hello World!\n");
	md = kzalloc(sizeof(*md), GFP_KERNEL);
	md->wq = create_singlethread_workqueue("my_single_thread");
	INIT_WORK(&md->my_work, my_workqueue_bottom_halves);
	ret = request_irq(IRQ_NUMBER, my_irq_handler,
						IRQF_SHARED,
						DRV_NAME, md);
	asm("int $0x3B");
	return 0;
}

static void __exit helloworld_exit(void)
{
	pr_info("End of the world!\n");
	cancel_work_sync(&md->my_work);
	free_irq(IRQ_NUMBER, md);
}

module_init(helloworld_init);
module_exit(helloworld_exit);
MODULE_AUTHOR("Toan Nguyen (toannguyenkt69@gmail.com)");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello World Module");