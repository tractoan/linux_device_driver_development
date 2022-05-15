#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>

static int my_pdrv_probe (struct platform_device *pdev){
	pr_info("Hello! device probed!\n");
	return 0;
}

static void my_pdrv_remove(struct platform_device *pdev){
	pr_info("good bye reader!\n");
}

static struct platform_driver my_driver = {
	.probe = my_pdrv_probe,
	.remove = my_pdrv_remove,
	.driver = {
		.name = "my_driver",
		.owner = THIS_MODULE,
	},
};

#ifndef USE_MODULE_PLATFORM_DRIVER_MACRO
static int __init my_driver_init(void)
{
	pr_info("Hello\n");
	/* Registering with Kernel */
	platform_driver_register(&my_driver);
	return 0;
}

static void __exit my_driver_remove (void)
{
	pr_info("Good bye\n");
	/* Unregistering from Kernel */
	platform_driver_unregister(&my_driver);
}

module_init(my_driver_init);
module_exit(my_driver_remove);
#else
module_platform_driver(my_driver);
#endif

MODULE_AUTHOR("Toan Nguyen (toannguyenkt69@gmail.com)");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello World Module");