#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/workqueue.h> /* for work queue */
#include <linux/kmod.h>

static struct delayed_work shutdown_work;
static void delayed_shutdown(struct work_struct *work)
{
	char *cmd = "/sbin/shutdown";
	char *argv[] = {
						cmd,
						"-h",
						"now",
						NULL,
					};
	char *envp[] = {
						"HOME=/",
						"PATH=/sbin:/bin:/usr/sbin:/usr/bin",
						NULL,
					};
	call_usermodehelper(cmd, argv, envp, 0);
}

static int __init helloworld_init(void)
{
	pr_info("Hello World!\n");
	INIT_DELAYED_WORK(&shutdown_work, delayed_shutdown);
	schedule_delayed_work(&shutdown_work, msecs_to_jiffies(800));
	return 0;
}

static void __exit helloworld_exit(void)
{
	pr_info("End of the world!\n");
	cancel_delayed_work_sync(&shutdown_work);
}

module_init(helloworld_init);
module_exit(helloworld_exit);

MODULE_AUTHOR("Toan Nguyen (toannguyenkt69@gmail.com)");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello World Module");