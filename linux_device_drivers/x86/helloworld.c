#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h> /* for tasklets API */

/*
Tasklets are a bottom-half (we will see what this means later) mechanism built on top of
softirqs. They are represented in the kernel as instances of struct tasklet_struct:
	struct tasklet_struct
	{
		struct tasklet_struct *next;
		unsigned long state;
		atomic_t count;
		void (*func)(unsigned long);
		unsigned long data;
	};
Tasklets are not re-entrant by nature. A code is called reentrant if it can be interrupted
anywhere in the middle of its execution, and then be safely called again. Tasklets are
designed such that a tasklet can run on one and only one CPU simultaneously (even on an
SMP system), which is the CPU it was scheduled on, but different tasklets may be run
simultaneously on different CPUs. The tasklet API is quite basic and intuitive.
*/
#define STATIC_METHOD

unsigned long tasklet_data=1234;

/* Tasklet handler, that just print the data */
void tasklet_work(struct tasklet_struct *tasklet)
{
printk("Call from Tasklet %ld\n", tasklet->data);
}

#ifdef STATIC_METHOD
DECLARE_TASKLET(my_tasklet, tasklet_work);
// DECLARE_TASKLET_DISABLED(my_tasklet, tasklet_function);
#else
struct tasklet_struct my_tasklet;
#endif

static int __init helloworld_init(void)
{
	pr_info("Hello World!\n");
	// Init Tasklet
	// void tasklet_init(struct tasklet_struct *t,
	// 					void (*func)(unsigned long), unsigned long data);
#ifdef STATIC_METHOD
	my_tasklet.data = tasklet_data;
#else
	tasklet_init(&my_tasklet, tasklet_work, tasklet_data);
#endif
	// Enable the Tasklet
	// void tasklet_enable(struct tasklet_struct *);
	tasklet_enable(&my_tasklet);

	// Disable the Tasklet
	// void tasklet_disable(struct tasklet_struct *); //disable the tasklet and return only when the tasklet has terminated its execution (if it was running)
	// void tasklet_disable_nosync(struct tasklet_struct *); returns immediately, even if the termination has not occurred
	// tasklet_disable(&my_tasklet); 	
	// tasklet_disable_nosync(&my_tasklet);  
	
	// Tasklet scheduling
	// void tasklet_schedule(struct tasklet_struct *t);	//normal priority
	// void tasklet_hi_schedule(struct tasklet_struct *t); //high priority
	tasklet_schedule(&my_tasklet);
	// tasklet_hi_schedule(&my_tasklet);

	/*The kernel maintains normal priority and high priority tasklets in two different lists.
tasklet_schedule adds the tasklet into the normal priority list, scheduling the associated
softirq with a TASKLET_SOFTIRQ flag. With tasklet_hi_schedule, the tasklet is added
into the high priority list, scheduling the associated softirq with a HI_SOFTIRQ flag. High
priority tasklets are meant to be used for soft interrupt handlers with low latency
requirements. There are some properties associated with tasklets you should know:
	-	Calling tasklet_schedule on a tasklet already scheduled, but whose execution
	has not started, will do nothing, resulting in the tasklet being executed only once.
	-	tasklet_schedule can be called in a tasklet, meaning that a tasklet can
	reschedule itself.
	-	High priority tasklets are always executed before normal ones. Abusive use of
	high priority tasks will increase the system latency. Only use them for really
	quick stuff.*/

	/*You can stop a tasklet using the tasklet_kill function that will prevent the tasklet from
running again or wait for its completion before killing it if the tasklet is currently scheduled
to run:*/
	// void tasklet_kill(struct tasklet_struct *t);

	return 0;
}

static void __exit helloworld_exit(void)
{
	pr_info("End of the world!\n");
	tasklet_kill(&my_tasklet);
}

module_init(helloworld_init);
module_exit(helloworld_exit);

MODULE_AUTHOR("Toan Nguyen (toannguyenkt69@gmail.com)");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello World Module");