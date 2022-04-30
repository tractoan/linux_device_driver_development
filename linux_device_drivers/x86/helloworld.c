#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

//container_of(pointer, container_type, container_field)

struct person {
	int age;
	char *name;
};

struct family {
	struct person father;
	struct person mother;
	struct person son;
	struct person daughter;
};

struct family my_family;

struct person *person_ptr = &my_family.father;

static int __init helloworld_init(void)
{
	pr_info("Hello World!\n");
	struct family *fam_ptr = container_of(person_ptr, struct family, father);
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
