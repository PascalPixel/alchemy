#include "types.h"
#include "iwram_call.h"
#include "callback_scheduler.h"
#include "mem.h"

void Func_0813f8d4(void);
void BattleFx_TransferHalvedCanvas(void);
void BattleFx_DimBackgroundStep(void);

/// Ends the light-streak backdrop: removes its three callbacks, clears the
/// canvas in VRAM and frees heap blocks 96 and 92.
void BattleFx_EndElementStreaks(void) {
  Scheduler_RemoveCallback(Func_0813f8d4);
  Scheduler_RemoveCallback(BattleFx_TransferHalvedCanvas);
  Scheduler_RemoveCallback(BattleFx_DimBackgroundStep);
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
