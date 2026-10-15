#include "types.h"
#include "iwram_call.h"
#include "callback_scheduler.h"
#include "mem.h"

void Func_0813f8d4(void);
void Func_0813f89c(void);
void Func_0813fa44(void);

void Func_0813fd40(void) {
  Scheduler_RemoveCallback(Func_0813f8d4);
  Scheduler_RemoveCallback(Func_0813f89c);
  Scheduler_RemoveCallback(Func_0813fa44);
  {
    s32 size = 0x4000;

    /* FAKEMATCH: the native loads the routine before it finishes the size
     * (differ 2 without the barrier). */
    __asm__("" : "+r"(size));
    Iwram_ClearWords((void *)0x06004000, size);
  }
  Runtime_ReleaseHeapBlock(96);
  Runtime_ReleaseHeapBlock(92);
}
