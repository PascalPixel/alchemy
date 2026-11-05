// Draft, differ 46 of 196, skeleton gap 36 (tla-en, 2026-11-01): when actor `id`
// stands at x 12, inside an event: the actor drops in place, then for 25 frame
// steps of three frames charges up (its sprite's byte 0x11) and every fourth
// step throws up a splash of type 14 at a random spot within 2 pixels
// (Func_02002808), then releases two objects of type 269 15 frames apart as
// Func_02002144 does, settles (Func_020028d0) and sets flag 0x8e5.
// Left: one constant. The level store's QI 3 is reused by CSE for the priority
// copy's mask (the path runs through the if), so here it is a pseudo in r11
// (4 refs over 116) and `id` loses r11 to the stack; the native reloads that 3
// and keeps `id` in r11. Also k's own `movs #3` is separate from the mask's, where
// the native shares one, and `zero` is set one call early (below).
// Tried, with what each proved. The native reads as a for loop with k after the
// level store: one shared SI 3, and only the priority's QI 3 hoisted (3 refs over
// 158, below `id`, so rematerialized), which that form gives (differ 115, gap 41).
// But there global.c ranks k (12 refs over 51, 0.71) above the actor (22 over 138,
// 0.64) and they swap r7 and r8; k needs 57 insns. k first gives 57, so this
// draft has the actor in r7, but in a for loop.c's combine_movables merges k's SI 3
// with the wider-or-equal QI 3s, moves all three uses out (7 refs over 156) and
// that takes r11 (differ 82, gap 44). The goto loop stops the merge. k placed
// after the byte 26 update hoists SI 3 (lifetime over 6: 12 * 6 < 81 real insns is
// the limit) and shortens k. No `k`, `k = i; k &= 3` (CSE folds it back), u8 or u32
// k, a raw `(unk11 & 3)` mask, a local for the byte (SI 3 then reaches the priority
// too), a u16 Charge unit, a y local, and `zero` in the splash branch (canon_reg
// folds it to k) gain nothing. Storing nextFrame after the priority copy puts k
// at 61 and everything in place except that store's order (differ 85, gap 29); the
// native's second loop shows that order is wrong. So does a look at the sums: k's
// rank needs about 10 more insns if they also lengthen the actor, and no RTL form
// of the listing's instructions has them.
// `zero` has to be a local set before Func_02002800: as a constant it crosses one
// call, 4 * 1 < 6 refs, and caller-save puts it in r4 with a spill; set above the
// call it crosses two and takes r8. The native sets r8 just after that call, and
// why that also escapes r4 is open. `|= 8` and `&= ~1` as two statements closed 8.
#include "types.h"
#include "actor.h"
#include "field_sprite.h"

struct Charge {
  u8 pad[17];
  u8 low : 2;
  u8 level : 6;
};

extern u8 gData_02002ae8[];
struct Actor *Actor_GetById(u32 id);
void Func_02002898(void);
void Func_02002968(s32 mode);
void Func_020028a0(void);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
u32 Random16Far(void);
struct Actor *Func_02002808(s32 type, s32 x, s32 y, s32 z);
void Func_02002800(struct Actor *object, const u8 *script);
void Animation_ApplyChildArgument(void *sprite, s32 argument);
void Animation_ApplyChildValuesToRecord(void *sprite, s32 value);
void Animation_ApplyChildValue(void *sprite, s32 value);
void Func_020028d0(s32 id, s32 a, s32 b);
void GameFlag_SetBit(s32 flag);

void Func_0200013c(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  struct Actor *splash, *spark;
  struct FieldSprite *sprite, *splashSprite, *sparkSprite;
  s32 i, x, z, k, zero;

  if (actor->x >> 20 != 12)
    return;
  Func_02002898();
  Func_02002968(0);
  actor->motionFlags = 3;
  wait(5);
  actor->motionFlags = 0;
  sprite = actor->animation;
  actor->velocityY = 0;
  actor->y = 0x40000;
  i = 0;
do {
    k = i & 3;
    ((struct Charge *)sprite)->level = i;
    sprite->nextFrame |= 8;
    sprite->nextFrame &= ~1;
    sprite->unk19 = 1;
    if (k == 0) {
      x = actor->x + Random16Far() * 2 - Random16Far() * 2;
      z = actor->z + Random16Far() * 2 - Random16Far() * 2;
      splash = Func_02002808(14, x, actor->y, z);
      if (splash != 0) {
        splashSprite = splash->animation;
        splash->terrainHeight = actor->terrainHeight;
        Func_02002800(splash, gData_02002ae8);
        splash->motionFlags = 0;
        if (splashSprite != 0) {
          splashSprite->nextFrame = 0;
          Animation_ApplyChildArgument(splashSprite, 1);
          splashSprite->priority = sprite->priority;
          Animation_ApplyChildValuesToRecord(splashSprite, 13);
          Animation_ApplyChildValue(splashSprite, 8);
        }
      }
    }
    wait(3);
    i++;
} while (i <= 24);

  actor->animationKind = 0;
  actor->y = 0;
  for (i = 1; i >= 0; i--) {
    spark = Func_02002808(269, actor->x, actor->y, actor->z);
    if (spark != 0) {
      sparkSprite = spark->animation;
      Func_02002800(spark, gData_02002ae8);
      zero = 0;
      spark->motionFlags = zero;
      spark->terrainId = 1;
      if (sparkSprite != 0) {
        Animation_ApplyChildArgument(sparkSprite, 2);
        sparkSprite->nextFrame = zero;
        sparkSprite->priority = 2;
      }
    }
    wait(15);
  }
  FieldEvent_Wait(30);
  actor->animationKind = 1;
  Func_020028d0(id, 0, 0);
  Func_020028a0();
  GameFlag_SetBit(0x8e5);
}
