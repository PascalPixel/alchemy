#include "types.h"
#include "system.h"
#include "mem.h"
#include "callback_scheduler.h"

void Func_080eb6a0(void);
void Func_080eb594(void);

/// Ends the canvas sprite: stops its draw and upload callbacks, waits a
/// frame and frees its sprite work, canvas and pixel map.
void BattleEffect_EndCanvasSprite(void) {
  Scheduler_RemoveCallback(Func_080eb6a0);
  Scheduler_RemoveCallback(Func_080eb594);
  WaitTicks(1);
  Runtime_ReleaseHeapBlock(180);
  Runtime_ReleaseHeapBlock(96);
  Runtime_ReleaseHeapBlock(240);
}
