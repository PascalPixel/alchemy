#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "iwram_call.h"
#include "event_work.h"

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
