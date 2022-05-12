/*
	Character devices are populated in the /dev directory. Do note that, they are not only files
present in that directory. A character device file is recognizable to its type, which we can
display thanks to the command ls -l. Major and minor identify and tie the devices with
the drivers. Let us see how it works, by listing the content of the /dev directory (ls -l
/dev):
		[...]
		drwxr-xr-x 2 root root 160 Mar 21 08:57 input
		crw-r----- 1 root kmem 1, 2 Mar 21 08:57 kmem
		lrwxrwxrwx 1 root root 28 Mar 21 08:57 log -> /run/systemd/journal/dev-log
		crw-rw---- 1 root disk 10, 237 Mar 21 08:57 loop-control
		brw-rw---- 1 root disk 7, 0 Mar 21 08:57 loop0
		brw-rw---- 1 root disk 7, 1 Mar 21 08:57 loop1
		brw-rw---- 1 root disk 7, 2 Mar 21 08:57 loop2
		brw-rw---- 1 root disk 7, 3 Mar 21 08:57 loop3
	Given the preceding excerpt, the first character of the first column identifies the file type.
Possible values are:
		c: This is for character device files
		b: This is for block device file
		l: This is for symbolic link
		d: This is for directory
		s: This is for socket
		p: This is for named pipe
	For b and c file types, the fifth and sixth columns right before the date respect the <X, Y>
pattern. X represents the major, and Y is the minor. For example, the third line is <1, 2>
and the last one is <7, 3>. That is one of the classical methods for identifying a character
device file from user space, as well as its major and minor.
The kernel holds the numbers that identify a device in dev_t type variables, which are
simply u32 (32-bit unsigned long). The major is represented with only 12 bits, whereas the
minor is coded on the 20 remaining bits.
	As one can see in include/linux/kdev_t.h, given a dev_t type variable, one may need
to extract the minor or the major. The kernel provides a macro for these purposes:
		MAJOR(dev_t dev);
		MINOR(dev_t dev);
	On the other hand, you may have a minor and a major, and need to build a dev_t. The
macro you should use is MKDEV(int major, int minor);:
		#define MINORBITS 20
		#define MINORMASK ((1U << MINORBITS) - 1)
		#define MAJOR(dev) ((unsigned int) ((dev) >> MINORBITS))
		#define MINOR(dev) ((unsigned int) ((dev) & MINORMASK))
		#define MKDEV(ma,mi) (((ma) << MINORBITS) | (mi))
	The device is registered with a major number that identifies the device, and a minor, which
one may use as an array index to a local list of devices, since one instance of the same driver
may handle several devices while different drivers may handle different devices of the
same type.
	Device numbers identify device files across the system. That means, there are two ways to
allocate these device numbers (actually major and minor):
	- 	Statically: Guessing a major not yet used by another driver using the
		register_chrdev_region() function. One should avoid using this as much as
		possible. Its prototype looks this:
			int register_chrdev_region(dev_t first, unsigned int count, \
										char *name);
		This method returns 0 on success, or a negative error code on failure. first is
		made of the major number that we need along with the first minor of the desired
		range. One should use MKDEV(ma,mi). count is the number of consecutive device
		numbers required, and name should be the name of the associated device or
		driver.
	-	Dynamically: Letting the kernel do the job for us, using the
		alloc_chrdev_region() function. This is the recommended way to obtain a
		valid device number. Its prototype is as follows:
			int alloc_chrdev_region(dev_t *dev, unsigned int firstminor, \
									unsigned int count, char *name);
		This method returns 0 on success, or a negative error code on failure. dev is the
		ony output parameter. It represents the first number the kernel assigned.
		firstminor is the first of the requested range of minor numbers, count the
		number of minors one requires, and name should be the name of the associated
		device or driver.

	The difference between the two is that with the former, one should know in advance what
number we need. This is registration: one tells the kernel what device numbers we want.
This may be used for pedagogic purposes, and works as long as the only user of the driver
is you. When it comes to loading the driver on another machine, there is no guarantee the
chosen number is free on that machine, and this will lead to conflicts and trouble. The
second method is cleaner and much safer, since the kernel is responsible for guessing the
right numbers for us. We do not even have to care about what the behavior would be on
loading the module on to another machine, since the kernel will adapt accordingly.
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