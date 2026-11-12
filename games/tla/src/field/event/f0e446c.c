#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "iwram_call.h"

struct EffectSprite {
  u8 unk00[4];
  u16 attr0_0 : 13;
  u16 color : 1;
  u16 attr0_14 : 2;
  u16 attr1;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
  s32 x;
  s32 y;
  s32 scaleX;
  s32 scaleY;
  u8 unk1c[2];
  u16 depth;
  u8 unk20[8];
};

struct EffectScene {
  u16 mode;
  u8 unk02[14];
  struct Actor *mainObject;
};

struct EffectParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer;
};

void Func_080eaf98(struct EffectSprite *sprite, s32 width, s32 height,
                   u32 shape, s32 tiles);
s32 BattleEffect_GetObjectDepth(struct Actor *object);
s32 BattleEffect_GetObjectPriority(struct Actor *object);
void Func_080eb01c(struct EffectSprite *sprite);
void Camera_WorldToScreen(s32 *position);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *position);
void BattleEffect_IntegrateVector2(struct EffectParticle *particle, s32 frames,
                                   s32 flags);
u32 Random16(void);
s32 Trig_Sin(s32 angle);

static inline s32 WrapClearWords(void *destination, s32 size) {
  /* FAKEMATCH(wrapper): the clear through this inline, so its arguments are set
   * before the entry's address is loaded; as a direct call, 4 of 77
   * instructions differ in tla-en. */
  return Iwram_ClearWords(destination, size);
}
void Func_080e446c(u8 *source, u8 *destination, s32 phase) {
  s32 width = 32;
  s32 height = 32;
  s32 column;
  s32 row;

  WrapClearWords(destination, 0x400);
  for (column = 0; column < width; column++) {
    s32 wave = (Trig_Sin(column << 13) << 3) + 0x80000;

    for (row = 0; row < height; row++) {
      s32 base = phase * 0x14ccc;
      s32 rowOffset = row * 0x40000 - 0x7c0000;
      s32 shift = base + rowOffset - wave;

      shift >>= 16;
      if (shift < 0) {
        destination[row * width + column] = source[row * width + column];
      } else if (shift + row < height) {
        destination[((shift + row) << 5) + column] =
            source[row * width + column];
      }
    }
  }
}

#include "event_work.h"

void Func_080e4510(struct EffectSprite *sprite, s32 tiles) {
  struct EffectScene *scene = Ram_HeapSlots->effectWork;
  struct EventWork *event = Ram_HeapSlots->eventWork;
  s32 position[3];

  Func_080eaf98(sprite, 16, 30, 0x80000000, tiles);
  sprite->depth = BattleEffect_GetObjectDepth(scene->mainObject);
  sprite->priority = BattleEffect_GetObjectPriority(scene->mainObject);
  sprite->color = 1;
  sprite->palette = 0;
  if (event->controlMode == 3) {
    sprite->scaleX = 0xc000;
    sprite->scaleY = 0xc000;
  } else {
    sprite->scaleX = scene->mainObject->scaleX;
    sprite->scaleY = scene->mainObject->scaleY;
  }
  position[0] = scene->mainObject->x;
  position[1] = scene->mainObject->y;
  position[2] = scene->mainObject->z;
  Camera_WorldToScreen(position);
  sprite->x = position[0];
  sprite->y = position[2];
}

/// Forwards to Func_080eb01c, which draws an effect sprite.
void Func_080e45a0(void *sprite) { Func_080eb01c(sprite); }

void Func_080e45a8(struct EffectSprite *sprite, struct EffectParticle *particle,
                   s32 unused, s32 tiles, s32 high) {
  struct Actor *mainObject =
      ((struct EffectScene *)Ram_HeapSlots->effectWork)->mainObject;
  s32 position[3];
  s32 y;

  position[0] = mainObject->x;
  position[1] = y = mainObject->y;
  position[2] = mainObject->z;
  if (high) {
    position[1] = y + 0x20000;
    Vector_AddPolarOffset(0x20000, Random16(), position);
  } else {
    Vector_AddPolarOffset(0x40000, Random16(), position);
  }
  particle->x = position[0];
  particle->y = position[1];
  particle->z = position[2];
  position[2] = 0;
  position[0] = 0;
  if (high)
    Vector_AddPolarOffset(0x3333, Random16(), position);
  else
    Vector_AddPolarOffset(0xcccc, Random16(), position);
  particle->velocityX = position[0];
  particle->velocityY = 0;
  particle->velocityZ = position[2];
  Func_080eaf98(sprite, 8, 8, 0x40000000, tiles);
  sprite->scaleX = 0x3333;
  sprite->scaleY = 0x3333;
  sprite->priority = BattleEffect_GetObjectPriority(mainObject);
  sprite->depth = BattleEffect_GetObjectDepth(mainObject) + 2;
  sprite->color = 1;
  sprite->palette = 0;
}
