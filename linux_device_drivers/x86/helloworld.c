#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init helloworld_init(void)
{
	pr_info("Hello World!\n");
	printk(KERN_EMERG "This is KERN_EMERG message\n");
	printk(KERN_ALERT "This is KERN_ALERT message\n");
	printk(KERN_CRIT "This is KERN_CRIT message\n");
	printk(KERN_ERR "This is KERN_ERR message\n");
	printk(KERN_WARNING  "This is KERN_WARNING  message\n");
	printk(KERN_NOTICE "This is KERN_NOTICE message\n");
	printk(KERN_INFO "This is KERN_INFO message\n");
	printk(KERN_DEBUG "This is KERN_DEBUG message\n");
	pr_info("This is pr_info message!\n");
	pr_err("This is pr_err message\n");
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
