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

/*HRT API
The required headers are:
	#include <linux/hrtimer.h>
An HRT is represented in the kernel as an instance of hrtimer:
	struct hrtimer {
		struct timerqueue_node node;
		ktime_t _softexpires;
		enum hrtimer_restart (*function)(struct hrtimer *);
		struct hrtimer_clock_base *base;
		u8 state;
		u8 is_rel;
	};
HRT setup initialization
Initializing the hrtimer: Before hrtimer initialization, you need to set up a ktime,1.
which represents time duration. We will see how to achieve that in the following
example:
	void hrtimer_init( struct hrtimer *time, clockid_t which_clock,
						enum hrtimer_mode mode);
Starting hrtimer: hrtimer can be started as shown in the following example:2.
	int hrtimer_start( struct hrtimer *timer, ktime_t time,
						const enum hrtimer_mode mode);
Kernel Facilities and Helper Functions
[ 60 ]
mode represents the expiry mode. It should be HRTIMER_MODE_ABS for an
absolute time value, or HRTIMER_MODE_REL for a time value relative to now.
hrtimer cancellation: You can either cancel the timer or see whether it is possible3.
to cancel it or not:
	int hrtimer_cancel( struct hrtimer *timer);
	int hrtimer_try_to_cancel(struct hrtimer *timer);
Both return 0 when the timer is not active and 1 when the timer is active. The
difference between these two functions is that hrtimer_try_to_cancel fails if
the timer is active or its callback is running, returning -1, whereas
hrtimer_cancel will wait until the callback finishes.
We can independently check whether the hrtimer's callback is still running with the
following:
	int hrtimer_callback_running(struct hrtimer *timer);
Remember, hrtimer_try_to_cancel internally calls hrtimer_callback_running.
In order to prevent the timer from automatically restarting, the hrtimer
callback function must return HRTIMER_NORESTART.
You can check whether HRTs are available on your system by doing the following:
By looking in the kernel config file, which should contain something like
CONFIG_HIGH_RES_TIMERS=y: zcat /proc/configs.gz | grep
CONFIG_HIGH_RES_TIMERS.
By looking at the cat /proc/timer_list or cat /proc/timer_list | grep
resolution result. The .resolution entry must show 1 nsecs and the
event_handler must show hrtimer_interrupts.
By using the clock_getres system call.
From within the kernel code, by using #ifdef CONFIG_HIGH_RES_TIMERS.
With HRTs enabled on your system, the accuracy of sleep and timer system calls do not
depend on jiffies anymore, but they are still as accurate as HRTs are. It is the reason why
some systems do not support nanosleep(), for example.
