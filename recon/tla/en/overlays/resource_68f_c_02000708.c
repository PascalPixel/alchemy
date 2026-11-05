// // Draft, differ 57 of 94 (tla-en, 2026-11-05); the goto loop it replaces measured
// // 53. Count random sparks around a position through the overlay's spawn
// // (Func_020000b8).
// // Left: the native computes velocityX in place before the next call, in r8, with
// // the mask in sl, the position in fp, x in r6 and count on the stack. Here combine
// // sinks the -8 and the shift through the calls to the argument, and local-alloc
// // takes velocityX into r6, which leaves x in r4 saved around its one call. Both
// // would follow from velocityX living across a block boundary in the native, which
// // no branch in the listing shows; an empty asm after it stops the sinking only.

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
    p->rotation = (Random16Far() & 15) << 12;
    velocityX = Random16Far();
    velocityX &= 15;
    velocityX -= 8;
    velocityX <<= 14;
    velocityY = Random16Far() & 15;
    x = position->x - ((Random16Far() & 31) << 16) + 0xf0000;
    Func_020000b8(x, position->y + ((Random16Far() & 15) << 16), position->z,
                  velocityX, (velocityY + 8) << 14, 0, 0x15e0000,
                  (struct TundariaTowerEffectParams *)p);
  }
}
