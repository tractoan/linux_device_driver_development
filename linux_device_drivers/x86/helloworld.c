#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mutex.h>

#ifdef STATIC_METHOD
DEFINE_MUTEX(my_mutex);
#else
struct mutex my_mutex;
#endif

static int __init helloworld_init(void)
{
	pr_info("Hello World!\n");
#ifdef STATIC_METHOD
	
#else
	mutex_init(&my_mutex);
#endif

	// Lock method
	// void mutex_lock(struct mutex *lock);					//not return if receive signal
	// int mutex_lock_interruptible(struct mutex *lock); 	//return if receive any signal
	// int mutex_lock_killable(struct mutex *lock); 		//return if receive kill signal

	// Unlock method
	// void mutex_unlock(struct mutex *lock);

	// Check lock state
	// int mutex_is_locked(struct mutex *lock);

	// Lock if available, else return
	// int mutex_trylock(struct mutex *lock);

	mutex_lock(&my_mutex);
	mutex_unlock(&my_mutex);

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
