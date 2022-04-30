#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/list.h>

struct car {
	int door_number;
	char *color;
	char *model;
	struct list_head list; /* kernel's list structure */
};

#ifdef STATIC_METHOD
static LIST_HEAD(carlist);
#else
struct list_head carlist;
#endif

struct car *redcar;
struct car *bluecar;

static int __init helloworld_init(void)
{
	pr_info("Hello World!\n");

#ifdef STATIC_METHOD

#else
	INIT_LIST_HEAD(&carlist);
#endif

	redcar = kmalloc(sizeof(struct car), GFP_KERNEL);
	bluecar = kmalloc(sizeof(struct car), GFP_KERNEL);

	INIT_LIST_HEAD(&bluecar->list);
	INIT_LIST_HEAD(&redcar->list);

	list_add(&redcar->list, &carlist);
	list_add(&bluecar->list, &carlist);
	return 0;
}

static void __exit helloworld_exit(void)
{
	pr_info("End of the world!\n");
	list_del(&redcar->list);
	list_del(&bluecar->list);
}

module_init(helloworld_init);
module_exit(helloworld_exit);

MODULE_AUTHOR("Toan Nguyen (toannguyenkt69@gmail.com)");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello World Module");
