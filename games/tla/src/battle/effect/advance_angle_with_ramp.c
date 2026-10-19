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

/// Per-frame update: spins 1/8 turn a frame as it shrinks, rising a pixel a
/// frame; released once its scale falls below 0x3000.
// As ☀️'s BattleFx_UpdateDescendingObject.
void BattleFx_UpdateDescendingObject(struct ObjectRuntime *obj) {
  u32 scale;
  u32 scaleX;
  /* FAKEMATCH(pin): the shrink step pinned to r1 and held by a volatile
   * barrier, loaded from the pool before scaleY is read; as a plain s32 step
   * = -0x280, 2 of 27 instructions differ in tla-en (24 with the barrier and
   * no pin). */
  register s32 step asm("r1") = -0x280;

  asm volatile("" : "+r"(step));
  obj->scaleY += step;
  scaleX = obj->scaleX;
  /* FAKEMATCH(barrier): memory barriers after the scaleX read and after the
   * spin, so the angle loads before the step is subtracted from scaleX and y
   * before 1 << 16 is built; as plain statements, 2 (first) and 4 (second) of
   * 27 instructions differ in tla-en. */
  asm("" : : : "memory");
  scale = scaleX + step;
  obj->angle += 0x2000;
  asm("" : : : "memory");
  obj->y += 1 << 16;
  obj->scaleX = scale;
  if ((s32)scale < 0x3000) {
    /* FAKEMATCH(pin): the zero pinned to r3 behind a barrier holding r2, as in
     * canvas.c, so it loads from the pool before the field's address; as
     * obj->animationKind = 0, 2 of 27 instructions differ in tla-en. */
    register u16 none asm("r3") = 0;

    asm("" : "+l"(none) : : "r2");
    obj->animationKind = none;
  }
}
