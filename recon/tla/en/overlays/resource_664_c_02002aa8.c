// Draft, differ 8 of 77 (was 27): a burst of eight effects that shrink from
// twice to half size, scattered over 64 by 16 pixels around (x, z) and rising
// at random speeds, then two screen shakes (Func_02005dcc). The loop counts
// up as a goto loop (as a for loop loop.c reverses it) and the scales are
// stored in field order. Left: the native copies x and z to r10 and r8 after
// the options' stores and builds the spawn position's address after the first
// random call, where this draft does both earlier (the parameter spill before
// the base address that no C form has changed); 12 minutes of permute found
// nothing better.
#include "types.h"
#include "system/field_effect.h"
#include "system/vector3.h"
#include "call.h"

u16 Random16Far(void);
void Func_020002f4(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Func_02005dcc(s32 a, s32 b, s32 c);

void Func_02002aa8(s32 x, s32 z) {
  struct EffectOptions options;
  struct Vector3 spawn;
  s32 i;

  options.priority = 1;
  options.startScaleX = 0x20000;
  options.startScaleY = 0x20000;
  options.targetScaleX = 0x8000;
  options.targetScaleY = 0x8000;
  i = 0;
burst:
    spawn.x = x + ((Random16Far() & 63) << 16) - 0x200000;
    spawn.y = (Random16Far() & 15) << 12;
    spawn.z = z + ((Random16Far() & 7) << 16) - 0x100000;
    Func_020002f4(spawn.x, 0, spawn.z, 0, spawn.y, 0,
                  EFFECT_SCALE_TO_TARGET | EFFECT_USE_START_SCALE |
                      EFFECT_USE_PRIORITY | 1,
                  &options);
  i++;
  if (i != 8)
    goto burst;
  Func_02005dcc(0x50000, 0x70000, 0x10000);
  Call3((void (*)())Func_02005dcc, -1, -1, 0xe666);
}
