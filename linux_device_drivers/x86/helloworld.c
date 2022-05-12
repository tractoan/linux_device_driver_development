/*
	Character devices transfer data to or from a user application by means of characters, in a
stream manner (one character after another), like a serial port does. A character device
driver exposes the properties and functionalities of a device by means of a special file in the
/dev directory, which one can use to exchange data between the device and user
application, and also allows you to control the real physical device. This is the basic concept
of Linux that says everything is a file. A character device driver represents the most basic
device driver in the kernel source. Character devices are represented in the kernel as
instances of struct cdev, defined in include/linux/cdev.h:
	struct cdev {
		struct kobject kobj;
		struct module *owner;
		const struct file_operations *ops;
		struct list_head list;
		dev_t dev;
		unsigned int count;
	};
*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

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