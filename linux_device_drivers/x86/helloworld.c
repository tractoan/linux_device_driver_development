/*
	Operations that one can perform on files depend on the drivers that manage those files.
Such operations are defined in the kernel as instances of struct file_operations.
struct file_operations exposes a set of callbacks that will handle any user-space
system call on a file. For example, if one wants users to be able to perform a write on the
file representing our device, one must implement the callback corresponding to that write
function and add it into the struct file_operations that will be tied to your device.
	Let's fill in a file operations structure:
		struct file_operations {
			struct module *owner;
			loff_t (*llseek) (struct file *, loff_t, int);
			ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
			ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
			unsigned int (*poll) (struct file *, struct poll_table_struct *);
			int (*mmap) (struct file *, struct vm_area_struct *);
			int (*open) (struct inode *, struct file *);
			long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
			int (*release) (struct inode *, struct file *);
			int (*fsync) (struct file *, loff_t, loff_t, int datasync);
			int (*fasync) (int, struct file *, int);
			int (*lock) (struct file *, int, struct file_lock *);
			int (*flock) (struct file *, int, struct file_lock *);
			[...]
		};
	Each of these callbacks is linked with a system
call, and none of them is mandatory. When a user code calls a files-related system call on a
given file, the kernel looks for the driver responsible for that file (especially the one that
created the file), locates its struct file_operations structure, and checks whether the
method that matches the system call is defined or not. If yes, it simply runs it. If not, it
returns an error code that varies depending on the system call. For example, an undefined
(*mmap) method will return -ENODEV to user, whereas an undefined (*write) method
will return -EINVAL.

	
	File representation in the kernel
	The kernel describes files as instances of struct inode (not struct file) structure, defined in
include/linux/fs.h:
		struct inode {
			[...]
			struct pipe_inode_info *i_pipe;  // Set and used if this is a linux kernel pipe 
			struct block_device *i_bdev; // Set and used if this is a block device
			struct cdev *i_cdev; // Set and used if this is a character device 
			[...]
		}
	The struct inode is a filesystem data structure holding information, which is only
relevant to the OS, about a file (whatever its type, character, block, pipe, and so on) or
directory (yes!! from a kernel point of view, a directory is a file that on entry points to other
files) on disk.
	The struct file structure (also defined in include/linux/fs.h) is actually a higher
level of file description that represents an open file in the kernel and which relies on the
lower struct inode data structure:
		struct file {
			[...]
			struct path f_path; // Path to the file 
			struct inode *f_inode; // inode associated to this file
			const struct file_operations *f_op; // operations that can be performed on this file
			loff_t f_pos; // Position of the cursor in this file
			// needed for tty driver, and maybe others
			void *private_data; // private data that driver can set in order to share some data between file operations. 
								// This can point to any data structure.
			[...]
		}
	The difference between struct inode and struct file is that an inode doesn't track the
current position within the file or the current mode. It only contains stuff that helps the OS
find the contents of the underlying file structure (pipe, directory, regular disk file,
block/character device file, and so on). On the other hand, the struct file is used as a
generic structure (it actually holds a pointer to a struct inode structure) that represents
and open file and provides a set of functions related to methods one can perform on the
underlying file structure. Such methods are: open, write, seek, read, select, and so on.
All this reinforces the philosophy of UNIX systems that says everything is file.
In other words, a struct inode represents a file in the kernel, and a struct file
describes it when it is actually open. There may be different file descriptors that represent
the same file opened several times, but these will point to the same inode.

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