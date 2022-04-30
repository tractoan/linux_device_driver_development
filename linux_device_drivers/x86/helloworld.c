#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>

static int __init helloworld_init(void)
{
	pr_info("Hello World!\n");
	pr_info("ndelay: 10000ns busy loop\n"); 	
	ndelay(10000);
	pr_info("udelay: 10000us busy loop\n");
	udelay(10000);
	pr_info("mdelay: 1000ms busy loop\n");
	mdelay(1000);
	pr_info("usleep_range: 1000000-2000000us sleep\n");
	usleep_range(1000000, 2000000);
	pr_info("msleep: 1000ms sleep\n");
	msleep(1000);
	pr_info("Module Init Done!\n");
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
