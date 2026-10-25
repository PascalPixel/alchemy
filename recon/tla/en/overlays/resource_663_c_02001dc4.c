// Draft, differ 210 of 193: a rockfall at (x, z): twelve dust effects scattered
// over 64 by 64 pixels (every fourth growing, the rest shrinking from twice
// size), six rocks of type 255 (the first's sprite shared by the rest,
// Func_02003c90) thrown out in random directions with random lift, then two
// screen shakes (Func_02003b98). Left: the native keeps x and z in r9 and r10, the first rock in r11 and the loop counts in r8, its frame is 4 bytes smaller, and its dust loop computes the scatter in place; the alignment is poor.
#include "types.h"
#include "object_runtime.h"
#include "iwram_call.h"
#include "field_sprite.h"
#include "system/field_effect.h"
#include "system/vector3.h"

u16 Random16Far(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Func_02003c90(void *sprite, void *source);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Func_02003b38(struct ObjectRuntime *object, const u8 *script);
void Func_02003b28(struct ObjectRuntime *object, s32 mode);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Func_02003b58(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02003b98(s32 a, s32 b, s32 c);
extern u8 gData_02003dfc[];

void Func_02001dc4(s32 x, s32 z) {
  struct ObjectRuntime *first, *rock;
  struct EffectOptions options;
  struct Vector3 v;
  s32 i;

  first = (struct ObjectRuntime *)Engine_ObjectCreate(255, x, 0, z);
  options.priority = 1;
  options.startScaleX = 0x20000;
  options.startScaleY = 0x20000;
  i = 0;
dust:
  {
    v.x = x + ((Random16Far() & 63) << 16) - 0x200000;
    v.y = (Random16Far() & 15) << 12;
    v.z = z + ((Random16Far() & 63) << 16) - 0x300000;
    if (i & 3)
      Func_0200015c(v.x, 0, v.z, 0, v.y, 0,
                    EFFECT_USE_PALETTE | EFFECT_SCALE_TO_TARGET | 1, &options);
    else
      Func_0200015c(v.x, 0, v.z, 0, v.y, 0, EFFECT_USE_PRIORITY | 1,
                    &options);
  }
  i++;
  if (i != 12)
    goto dust;
  i = 0;
rocks:
  {
    rock = first;
    if (i != 0) {
      rock = (struct ObjectRuntime *)Engine_ObjectCreate(255, x, 0, z);
      Func_02003c90(rock->animation, first->animation);
    }
    ObjectDispatch_SetSingleChildFlags(rock, 0);
    Func_02003b38(rock, gData_02003dfc);
    Func_02003b28(rock, 2);
    rock->velocityY = ((Random16Far() & 3) << 16) + 0x40000;
    rock->gravity = 0x3333;
    rock->bounce = 0x6666;
    rock->scaleX = rock->scaleY = rock->speedLimit = 0xcccc;
    v.y = Random16Far() & 7;
    v.x = Iwram_MulQ16(Math_Cosine(v.y << 12), 0x400000);
    v.z = Iwram_MulQ16(Math_Sine(v.y << 12), 0x400000);
    ((struct FieldSprite *)rock->animation)->priority = 1;
    Func_02003b58(rock, rock->x + v.x, rock->y, rock->z + v.z);
  }
  i++;
  if (i != 6)
    goto rocks;
  Func_02003b98(0x50000, 0x70000, 0x10000);
  Func_02003b98(-1, -1, 0xe666);
}
