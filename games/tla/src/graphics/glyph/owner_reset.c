#include "types.h"
#include "callback_scheduler.h"

void Func_0803df24(void);

void Func_0803df00(void) {
  Scheduler_AddOrUpdateCallback(Func_0803df24, 0x480);
}

void Resource_ScheduleOwnerReset(void) {
  Scheduler_RemoveCallback(Func_0803df24);
}
