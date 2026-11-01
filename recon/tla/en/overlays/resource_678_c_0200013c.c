// Draft, differ 122 of 196, skeleton gap 46 (tla-en, 2026-11-01): when actor `id`
// stands at x 12, inside an event: the actor drops in place, then for 25 frame
// steps of three frames charges up (its sprite's byte 0x11) and every fourth
// step throws up a splash of type 14 at a random spot within 2 pixels
// (Func_02002808), then releases two objects of type 269 15 frames apart as
// Func_02002144 does, settles (Func_020028d0) and sets flag 0x8e5.
// Left: nearly all of the gap is one allocation pair. The native has the actor
// in r7 and `k` in r8; here global.c ranks `k` (12 refs over 51 insns) above the
// actor (22 over 139), so they swap and every `k` and actor access gains or
// loses a reload. `k` needs about 57 insns to fall below the actor. Setting `k`
// before the charge byte gives that (actor r7) but loop.c then matches the two
// 3s and hoists them, costing `id` its register (gap 52, differ 108). Placing
// `k` later hoists as well. Random16Far's sums as one expression, the charge
// byte as a bitfield and u32 Random16Far came from the listing's shape; loop
// form (do-while), separate second-loop locals, u32 or narrow `k` and all orders
// of the four sprite statements do not reach it. The -13 from 3 in the priority
// copy is reload_cse's and should follow once registers do.
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
  u8 *raw, *kind;
  s32 i, x, z, k;

  if (actor->x >> 20 != 12)
    return;
  Func_02002898();
  Func_02002968(0);
  actor->motionFlags = 3;
  wait(5);
  actor->motionFlags = 0;
  sprite = actor->animation;
  raw = (u8 *)sprite;
  actor->velocityY = 0;
  actor->y = 0x40000;
  for (i = 0; i <= 24; i++) {
    ((struct Charge *)raw)->level = i;
    k = i & 3;
    raw[26] = (raw[26] | 8) & ~1;
    raw[25] = 1;
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
          Animation_ApplyChildArgument(splashSprite, 1);
          ((u8 *)splashSprite)[26] = 0;
          splashSprite->priority = sprite->priority;
          Animation_ApplyChildValuesToRecord(splashSprite, 13);
          Animation_ApplyChildValue(splashSprite, 8);
        }
      }
    }
    wait(3);
  }
  kind = &actor->animationKind;
  *kind = 0;
  actor->y = 0;
  for (i = 1; i >= 0; i--) {
    spark = Func_02002808(269, actor->x, actor->y, actor->z);
    if (spark != 0) {
      sparkSprite = spark->animation;
      Func_02002800(spark, gData_02002ae8);
      spark->motionFlags = 0;
      spark->terrainId = 1;
      if (sparkSprite != 0) {
        Animation_ApplyChildArgument(sparkSprite, 2);
        ((u8 *)sparkSprite)[26] = 0;
        sparkSprite->priority = 2;
      }
    }
    wait(15);
  }
  FieldEvent_Wait(30);
  *kind = 1;
  Func_020028d0(id, 0, 0);
  Func_020028a0();
  GameFlag_SetBit(0x8e5);
}
