/*
An interrupt handler will always have priority on the user task, even if that task is holding a
spinlock. Simply disabling IRQ is not sufficient. An interrupt may happen on another CPU.
It would be a disaster if a user task updating the data gets interrupted by an interrupt
handler trying to access the same data. Using spin_lock_irqsave() will disable all
interrupts on the local CPU, preventing the system call from being interrupted by any kind
of interrupt
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static irqreturn_t my_interrupt_handler(int irq, void *p)
{
	/*
	* preemption is disabled when running interrupt handler
	* also, the serviced irq line is disabled until the handler has
	completed
	* no need then to disable all other irq. We just use spin_lock and
	* spin_unlock
	*/
	spin_lock(&my_lock);
	/* process data */
	[...]
	spin_unlock(&my_lock);
	return IRQ_HANDLED;
}

ssize_t my_read(struct file *filp, char __user *buf, size_t count,loff_t *f_pos)
{
	unsigned long flags;
	/* some stuff */
	[...]
	unsigned long flags;
	spin_lock_irqsave(&my_lock, flags);
	data++;
	spin_unlock_irqrestore(&my_lock, flags)
	[...]
}

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