// Matches: differ 0 of 193 (tla-en, 2026-11-06).
// A rockfall at (x, z): twelve dust effects scattered over 64 by 64 pixels
// (three of every four start at twice size), six rocks of type 255 (the first's
// sprite shared by the rest, Func_02003c90) thrown out in random directions with
// random lift, then two screen shakes (Func_02003b98).
// radius, shakeA and shakeC each hold one constant for their whole life: written
// inline, a literal 0x400000 is shared across the calls by CSE and spills
// `first`. The scatter vector is used directly (a pointer to it was hoisted out
// of the dust loop), and both loops are `for` loops ending on `!=`, as the
// listing's bne shows.
#include "types.h"
#include "call.h"
#include "actor.h"
#include "iwram_call.h"
#include "field_sprite.h"
#include "system/field_effect.h"
#include "system/vector3.h"

u32 Random16Far(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Func_02003c90(void *sprite, void *source);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Func_02003b38(struct Actor *object, const u8 *script);
void Func_02003b28(struct Actor *object, s32 mode);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Func_02003b58(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02003b98(s32 a, s32 b, s32 c);
extern u8 gData_02003dfc[];

void Func_02001dc4(s32 x, s32 z) {
  struct Actor *first, *rock;
  struct EffectOptions options;
  struct Vector3 scatter;
  s32 i;
  s32 radius = 0x400000;
  s32 shakeA = -1;
  s32 shakeC = 0xe666;

  first = (struct Actor *)Import_ActorCreate(255, x, 0, z);
  options.priority = 1;
  options.startScaleX = 0x20000;
  options.startScaleY = 0x20000;
  for (i = 0; i != 12; i++) {
    scatter.x = x + ((Random16Far() & 63) << 16) - 0x200000;
    scatter.y = (Random16Far() & 15) << 12;
    scatter.z = z + ((Random16Far() & 63) << 16) - 0x300000;
    if (i & 3)
      Func_0200015c(scatter.x, 0, scatter.z, 0, scatter.y, 0,
                    EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | 1, &options);
    else
      Func_0200015c(scatter.x, 0, scatter.z, 0, scatter.y, 0,
                    EFFECT_USE_PRIORITY | 1, &options);
  }
  for (i = 0; i != 6; i++) {
    rock = first;
    if (i != 0) {
      rock = (struct Actor *)Import_ActorCreate(255, x, 0, z);
      Func_02003c90(rock->animation, first->animation);
    }
    Actor_SetSingleChildFlags(rock, 0);
    Func_02003b38(rock, gData_02003dfc);
    Func_02003b28(rock, 2);
    rock->velocityY = ((Random16Far() & 3) << 16) + 0x40000;
    rock->gravity = 0x3333;
    rock->bounce = 0x6666;
    rock->scaleX = 0xcccc;
    rock->scaleY = 0xcccc;
    rock->speedLimit = 0xcccc;
    scatter.y = Random16Far() & 7;
    scatter.x = Iwram_MulQ16(Math_Cosine(scatter.y << 12), radius);
    scatter.z = Iwram_MulQ16(Math_Sine(scatter.y << 12), radius);
    ((struct FieldSprite *)rock->animation)->priority = 1;
    Func_02003b58(rock, rock->x + scatter.x, rock->y, rock->z + scatter.z);
  }
  Func_02003b98(0x50000, 0x70000, 0x10000);
  Func_02003b98(shakeA, shakeA, shakeC);
}
