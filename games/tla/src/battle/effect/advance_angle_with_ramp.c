#include "types.h"
#include "scene.h"
#include "motobj.h"
#include "object_runtime.h"

void BattleFx_AdvanceAngleWithRamp(struct ObjectRuntime *object) {
  u32 step;

  step = object->action * 0x50;
  object->angle += step + 0x1000;
  if (step < 0x1000)
    object->action++;
}

// Per-frame update: spins 1/8 turn a frame as it shrinks, rising a pixel a
// frame; released once its scale falls below 0x3000, as ☀️'s
// BattleFx_UpdateDescendingObject.
void BattleFx_UpdateDescendingObject(struct ObjectRuntime *obj) {
  u32 scale;
  u32 scaleX;
  /* FAKEMATCH: the shrink step pinned to r1 behind a volatile barrier: the
   * native loads it from the pool before it reads scaleY (plainly the whole
   * function differs by 8; with the barrier but no pin, 24). */
  register s32 step asm("r1") = -0x280;

  asm volatile("" : "+r"(step));
  obj->scaleY += step;
  scaleX = obj->scaleX;
  /* FAKEMATCH: memory barriers after the scaleX read and after the spin: the
   * native loads the angle before it subtracts the step from scaleX, and y
   * before it builds 1 << 16 (differ 2 without the first, 4 without the
   * second). */
  asm("" : : : "memory");
  scale = scaleX + step;
  obj->angle += 0x2000;
  asm("" : : : "memory");
  obj->y += 1 << 16;
  obj->scaleX = scale;
  if ((s32)scale < 0x3000) {
    /* FAKEMATCH: the zero pinned to r3 behind a barrier that holds r2, as in
     * canvas.c: the native loads it from the pool before it starts the
     * field's address (differ 2 without it). */
    register u16 none asm("r3") = 0;

    asm("" : "+l"(none) : : "r2");
    obj->animationKind = none;
  }
}
