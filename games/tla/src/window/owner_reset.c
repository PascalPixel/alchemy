#include "types.h"
#include "task.h"

void Func_0803df24(void);

void Func_0803df00(void) {
  Task_Add(Func_0803df24, TASK_PASS_VBLANK);
}

void Resource_ScheduleOwnerReset(void) {
  Task_Remove(Func_0803df24);
}
