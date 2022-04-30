#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>

spinlock_t my_spinlock;

static int __init helloworld_init(void)
{
	pr_info("Hello World!\n");
	// Init
	// spin_lock_init(spinlock_t *lock);

	// Lock
	// void spin_lock(spinlock_t *lock);
	// int spin_trylock(spinlock_t *lock);
	// void spin_unlock(spinlock_t *lock);

	// Lock and Block Bottom Half
	// void spin_lock_bh(spinlock_t *lock);
	// int spin_trylock_bh(spinlock_t *lock)
	// void spin_unlock_bh(spinlock_t *lock);

	// Lock and Block IRQ
	// void spin_lock_irqsave(spinlock_t *lock);
	// int spin_trylock_irqsave(spinlock_t *lock);
	// void spin_unlock_irqrestore(spinlock_t *lock, unsigned long flags)
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
