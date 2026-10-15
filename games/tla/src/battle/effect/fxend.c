#include "types.h"
#include "system.h"
#include "mem.h"
#include "callback_scheduler.h"

void Func_080eb6a0(void);
void Func_080eb594(void);

void Func_080eb930(void) {
  Scheduler_RemoveCallback(Func_080eb6a0);
  Scheduler_RemoveCallback(Func_080eb594);
  WaitFrames(1);
  Runtime_ReleaseHeapBlock(180);
  Runtime_ReleaseHeapBlock(96);
  Runtime_ReleaseHeapBlock(240);
}
