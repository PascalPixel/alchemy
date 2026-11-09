#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "event_inline.h"

struct Charge {
  u8 pad[17];
  u8 low : 2;
  u8 level : 6;
};

extern u8 gData_02002ae8[];
void Func_02002898(void);
void Func_020028a0(void);
u32 Random16Far(void);
void Func_02002800(struct Actor *object, const u8 *script);
void Animation_ApplyChildArgument(void *sprite, s32 argument);
void Animation_ApplyChildValuesToRecord(void *sprite, s32 value);
void Animation_ApplyChildValue(void *sprite, s32 value);

// When actor id stands at x 12, inside an event: the actor drops in place, then
// for 25 frames throws splashes, then two sparks, and sets flag 0x8e5.
void Func_0200013c(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  struct Actor *splash, *spark;
  struct FieldSprite *sprite, *splashSprite, *sparkSprite;
  s32 i, x, z, k, zero;

  if (actor->x >> 20 != 12)
    return;
  Func_02002898();
  Event_ResolvePendingActions(0);
  actor->motionFlags = 3;
  wait(5);
  actor->motionFlags = 0;
  actor->velocityY = 0;
  actor->y = 0x40000;
  sprite = actor->animation;
  i = 0;
do {
    ((struct Charge *)sprite)->level = i;
    sprite->nextFrame |= 8;
    sprite->nextFrame &= ~1;
    k = i;
    sprite->unk19 = 1;
    k &= 3;
    if (k == 0) {
      x = actor->x + Random16Far() * 2 - Random16Far() * 2;
      z = actor->z + Random16Far() * 2 - Random16Far() * 2;
      splash = ((struct Actor *)Import_ActorCreate(14, x, actor->y, z));
      if (splash != 0) {
        splashSprite = splash->animation;
        splash->terrainHeight = actor->terrainHeight;
        Func_02002800(splash, gData_02002ae8);
        splash->motionFlags = 0;
        if (splashSprite != 0) {
          ((s32 (*)())Animation_ApplyChildArgument)(splashSprite, 1);
          splashSprite->nextFrame = k;
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
    spark = ((struct Actor *)Import_ActorCreate(269, actor->x, actor->y, actor->z));
    if (spark != 0) {
      sparkSprite = spark->animation;
      Func_02002800(spark, gData_02002ae8);
      {
        u8 *flags = &spark->motionFlags;

        zero = 0;
        *flags = zero;
      }
      spark->terrainId = 1;
      if (sparkSprite != 0) {
        Animation_ApplyChildArgument(sparkSprite, 2);
        sparkSprite->nextFrame = zero;
        sparkSprite->priority = 2;
      }
    }
    wait(15);
  }
  Event_Wait(30);
  actor->animationKind = 1;
  Actor_SetHorizontalPositionWithTerrain(id, 0, 0);
  Func_020028a0();
  GameFlag_SetBit(0x8e5);
}
