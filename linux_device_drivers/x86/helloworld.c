#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>

static char *my_str = "hello";
static int my_int = 1;
static int my_arr[3] = {0, 1, 2};

//module_param(name, type, perm);
//	name: name of variable
//	type: type of parameter (bool, byte, short, ushort, int, uint, long, ulong, charp for char pointer)
//	perm: permission, represent file permissions /sys/module/<module>/parameters/<param>. Some of them are S_IWUSR, 
//		S_IRUSR, S_IXUSR, S_IRGRP, S_WGRP, S_IRUGO, where:
//			S_I is prefix
//			R: Read, W: Write, X: Execute
//			USR: User, GRP: Group, UGO: User, Group and Others
//		Use '|' to set multiple Permission
module_param(my_int, int, S_IRUGO);
module_param(my_str, charp, S_IRUGO);
module_param_array(my_arr, int, NULL, S_IWUSR|S_IRUSR);

MODULE_PARM_DESC(my_int, "this is my int variable");
MODULE_PARM_DESC(my_str, "this is my char pointer variable");
MODULE_PARM_DESC(my_arr, "this is my array of int");

static int foo(void)
{
	pr_info("My string is a string: %s\n", my_str);
	pr_info("Array Elements: %d\t%d\t%d\n", my_arr[0], my_arr[1], my_arr[2]);
	pr_info("Integer Value: %d\n", my_int);
	return my_int;
}

static int __init helloworld_init(void)
{
	pr_info("Hello World!\n");
	foo();
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
