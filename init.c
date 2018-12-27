#include <linux/init.h>
#include <linux/proc_fs.h>
#include "elevator.h"

static struct file_operations fops;

//Function Pointers for syscalls
extern int (*STUB_start_elevator)(void);
extern int (*STUB_issue_request)(int, int, int);
extern int (*STUB_stop_elevator)(void);

static int elevator_init(void)
{
	elevatorInitLocks();
	floorInitLocks();
	STUB_start_elevator = & (startElevator);
	STUB_issue_request = & (issue_request);
	STUB_stop_elevator = & (stopElevator);
  
	initfloorlist();
	printk(KERN_NOTICE "/proc/%s create\n", ENTRY_NAME);
	fops.open = elevator_proc_open;
	fops.read = elevator_proc_read;
	fops.release = elevator_proc_release;

	if (!proc_create(ENTRY_NAME, PERMS, NULL, &fops))
	 {
		printk("ERROR! proc_create\n");
		remove_proc_entry(ENTRY_NAME, NULL);
		return -ENOMEM;
	 }
	 return 0;
}

module_init(elevator_init);

static void elevator_exit(void)
{
	STUB_start_elevator = NULL;
	STUB_issue_request = NULL;
	STUB_stop_elevator = NULL;

	stopElevator();
	remove_proc_entry(ENTRY_NAME, NULL);
	clearfloorlist();
	elevatorCleanLocks();
	floorCleanLocks();
	printk(KERN_NOTICE "Removing /proc/%s.\n", ENTRY_NAME);
}

module_exit(elevator_exit);		// added semi colon
