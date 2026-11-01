// Draft, differ 83 of 193, skeleton gap 25 (13 removed, 12 added) (tla-en,
// 2026-11-01)
// A rockfall at (x, z): twelve dust effects scattered over 64 by 64 pixels
// (three of every four start at twice size), six rocks of type 255 (the first's
// sprite shared by the rest, Func_02003c90) thrown out in random directions with
// random lift, then two screen shakes (Func_02003b98). Frame, prologue and
// register homes now agree with the listing.
// radius, shakeA and shakeC are locals set in an earlier block so that gcse's
// constant propagation loads each constant at its call: a literal 0x400000 is
// shared across the calls by CSE and spills `first`. Both loops stay label loops:
// a real loop hoists the pool loads and scores 40.
// Left, and measured: block copies of v->x, v->y and v->z, `next = i + 1` before
// the branch (with i = next after the join) and `low = i & 3` take the gap to 15
// (differ 67), which puts the loads and the increment before the branch as the
// listing has them. Those are forced temporaries (S2 `temporary`), left out while
// plain C is the limit. Call3 for the second shake closes its constants but is a
// device too.
// Not reached: the listing computes &scatter and &options inside the dust loop
// (r7, r4) and keeps r7 for the rock loop; gcse's PRE hoists both here, which
// also leaves the zero for i in r2 rather than r0. The priority store's
// `adds r2, r0, #0` and the reload of v->z: a u8 * store reloads v->z but loses
// the -13 mask.
#include "types.h"
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
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
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
  struct Vector3 *v;
  struct Vector3 scatter;
  s32 i;
  s32 radius = 0x400000;
  s32 shakeA = -1;
  s32 shakeC = 0xe666;

  first = (struct Actor *)Import_ActorCreate(255, x, 0, z);
  options.priority = 1;
  options.startScaleX = 0x20000;
  options.startScaleY = 0x20000;
  i = 0;
dust:
  v = &scatter;
  v->x = x + ((Random16Far() & 63) << 16) - 0x200000;
  v->y = (Random16Far() & 15) << 12;
  v->z = z + ((Random16Far() & 63) << 16) - 0x300000;
  {
    if (i & 3)
      Func_0200015c(v->x, 0, v->z, 0, v->y, 0,
                    EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | 1, &options);
    else
      Func_0200015c(v->x, 0, v->z, 0, v->y, 0, EFFECT_USE_PRIORITY | 1, &options);
  }
  i++;
  if (i != 12)
    goto dust;
  i = 0;
rocks:
  {
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
    v->y = Random16Far() & 7;
    v->x = Iwram_MulQ16(Math_Cosine(v->y << 12), radius);
    v->z = Iwram_MulQ16(Math_Sine(v->y << 12), radius);
    ((struct FieldSprite *)rock->animation)->priority = 1;
    Func_02003b58(rock, rock->x + v->x, rock->y, rock->z + v->z);
  }
  i++;
  if (i != 6)
    goto rocks;
  Func_02003b98(0x50000, 0x70000, 0x10000);
  Func_02003b98(shakeA, shakeA, shakeC);
}
