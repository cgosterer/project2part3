#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/module.h>

//Declares function pointers for syscalls and exports them for module usage
int (*STUB_start_elevator)(void) = NULL;
int (*STUB_issue_request)(int, int, int) = NULL;
int (*STUB_stop_elevator)(void) = NULL;
EXPORT_SYMBOL(STUB_start_elevator);
EXPORT_SYMBOL(STUB_issue_request);
EXPORT_SYMBOL(STUB_stop_elevator);

//start elevator wrapper
asmlinkage int sys_start_elevator(void) {
  if(STUB_start_elevator != NULL)
    return STUB_start_elevator();
  else
    return -ENOSYS;
}

//issue request wrapper
asmlinkage int sys_issue_request(int passengerType, int startFloor, int destFloor) {
  if(STUB_issue_request != NULL)
    return STUB_issue_request(passengerType, startFloor, destFloor);
  else
    return -ENOSYS;
}

//stop elevator wrapper
asmlinkage int sys_stop_elevator(void) {
  if(STUB_stop_elevator != NULL)
    return STUB_stop_elevator();
  else
    return -ENOSYS;
}
