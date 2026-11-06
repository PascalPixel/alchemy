// Differ 31 of 94 (tla-en, 2026-11-06), unchanged: options written directly rather than
// through p (31); the permuter's best in 8 minutes was 33.
// Draft, differ 31 of 94, draft 91 (tla-en, 5 November 2026). Count random sparks around a
// position through the overlay's spawn (Func_020000b8). A local mask set at the first use.
// Left: the native keeps velocityX in r8 through all three steps (in-place -8 and shift,
// each an in-out reload), mask r10, i r9. Written as separate statements, combine sinks the
// -8 and the shift through the calls to the argument, and a barrier does not bring them
// back (53 to 61). Here velocityX is r9, mask r8, i r10.

#include "../../../../games/tla/src/scene/tundaria_tower/tundaria_tower.h"

void Func_020006d4(union FieldObject *object);

void Func_02000708(struct TundariaTowerPosition *position, s32 count,
                   s32 scale) {
  struct EffectOptions options;
  struct EffectOptions *p = &options;
  s32 i;
  s32 velocityX;
  s32 velocityY;
  s32 x;

  p->priority = 1;
  p->type = 279;
  p->targetScaleY = 0xcccc;
  p->targetScaleX = 0xcccc;
  p->startScaleY = scale;
  p->startScaleX = scale;
  p->update = Func_020006d4;
  for (i = 0; i != count; i++) {
    s32 mask;
    p->rotation = (Random16Far() & 15) << 12;
    velocityX = Random16Far();
    velocityX &= mask = 15;
    velocityX = (velocityX - 8) << 14;
    velocityY = Random16Far() & mask;
    x = position->x - ((Random16Far() & 31) << 16) + 0xf0000;
    Func_020000b8(x, position->y + ((Random16Far() & mask) << 16), position->z,
                  velocityX, (velocityY + 8) << 14, 0, 0x15e0000,
                  (struct TundariaTowerEffectParams *)p);
  }
}
