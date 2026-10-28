#include "types.h"
#include "system.h"
#include "mem.h"
#include "task.h"

void Func_080eb6a0(void);
void Func_080eb594(void);

/// Ends the canvas sprite: stops its draw and upload callbacks, waits a
/// frame and frees its sprite work, canvas and pixel map.
void BattleEffect_EndCanvasSprite(void) {
  Task_Remove(Func_080eb6a0);
  Task_Remove(Func_080eb594);
  wait(1);
  HeapSlot_Release(180);
  HeapSlot_Release(96);
  HeapSlot_Release(240);
}
