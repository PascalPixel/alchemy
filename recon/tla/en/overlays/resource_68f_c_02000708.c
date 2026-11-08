// Matches (tla-en, 2026-11-08, -mtune=gba), and in all six editions.
// 2026-11-08: from differ 31, each velocity built in place, one statement a step: the
// x velocity masked, offset and shifted before the y velocity is drawn (velocityX in r8
// as the listing), and the y velocity offset and shifted after x is computed, which
// lengthens velocityX's life enough to keep it out of r6 (sched2 otherwise moves its
// offset and shift past the calls). Count random sparks around a position through the
// overlay's spawn (Func_020000b8).

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
    mask = 15;
    velocityX &= mask;
    velocityX -= 8;
    velocityX <<= 14;
    velocityY = Random16Far() & mask;
    velocityY += 8;
    velocityY <<= 14;
    x = position->x - ((Random16Far() & 31) << 16) + 0xf0000;
    Func_020000b8(x, position->y + ((Random16Far() & mask) << 16), position->z,
                  velocityX, velocityY, 0, 0x15e0000,
                  (struct TundariaTowerEffectParams *)p);
  }
}
