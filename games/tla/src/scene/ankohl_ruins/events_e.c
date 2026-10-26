#include "ankohl_ruins.h"

extern s16 gData_02003782;
extern s16 gData_02003784;
extern s32 gData_0200378c;

void Scheduler_RemoveCallbackFar(void (*callback)(void));

void Func_020015ac(void) {
  struct AnkohlRuinsEventWork *work =
      (struct AnkohlRuinsEventWork *)Ram_HeapSlots->eventWork;

  if (GameFlag_Test(0x104) == 0) {
    if (++gData_02003782 >= gData_02003784) {
      Scheduler_RemoveCallbackFar(Func_020015ac);
      if (gData_0200378c == 1)
        work->alarm = 68;
      else
        work->alarm = 69;
    }
  }
}
