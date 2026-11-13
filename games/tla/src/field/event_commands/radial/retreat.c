/// @file
/// Retreat.
// Part of radial.c.

#include "../radial_internal.h"
#include "event.h"
#include "event_work.h"
#include "module/field/event_commands.h"
#include "module/field/radial.h"
#include "scene_call.h"

#define SLOT_MAX 24

struct SpinScene {
  u8 unk00[0x10];
  struct SpinObject *mainObject;
  u8 unk14[4];
  s16 firstObjectId;
  u8 unk1a[0x36];
  struct FieldParticle slots[SLOT_MAX];
};

/* VIEW(launch): routine Animation_ApplyChildValuesFar as void Animation_ApplyChildValuesFar(void *object, s32 value); actor_view.h has void Animation_ApplyChildValuesFar(struct Actor *, u32). */
extern void Animation_ApplyChildValuesFar__launch(void *object, s32 value) __asm__("Animation_ApplyChildValuesFar");
/* VIEW(launch): routine Actor_SetSingleChildFlagsFar as void Actor_SetSingleChildFlagsFar(void *object, s32 flags); actor_view.h has void Actor_SetSingleChildFlagsFar(struct Actor *object, u32 flags). */
extern void Actor_SetSingleChildFlagsFar__launch(void *object, s32 flags) __asm__("Actor_SetSingleChildFlagsFar");
/* VIEW(launch): routine Camera_WorldToScreen as void Camera_WorldToScreen(struct SpinVector *position); age_and_cue has void Camera_WorldToScreen(struct EffectVector *pos). */
extern void Camera_WorldToScreen__launch(struct SpinVector *position) __asm__("Camera_WorldToScreen");
void Actor_ApplyRandomChildValues(struct SpinObject *owner);
/* VIEW(launch): routine BattleEffect_AdvanceAngleWithRamp as void BattleEffect_AdvanceAngleWithRamp(struct SpinObject *object); the definition has void BattleEffect_AdvanceAngleWithRamp(struct Actor *object). */
extern void BattleEffect_AdvanceAngleWithRamp__launch(struct SpinObject *object) __asm__("BattleEffect_AdvanceAngleWithRamp");
/* VIEW(launch): routine BattleEffect_UpdateDescendingObject as void BattleEffect_UpdateDescendingObject(struct SpinObject *obj); the definition has void BattleEffect_UpdateDescendingObject(struct Actor *obj). */
extern void BattleEffect_UpdateDescendingObject__launch(struct SpinObject *obj) __asm__("BattleEffect_UpdateDescendingObject");

void FieldEffect_RunRetreat(void) {
  struct SpinScene *scene = Ram_HeapSlots->effectWork;
  struct SpinObject *obj = scene->mainObject;
  struct SpinVector pos;
  struct FieldParticle *slot;
  struct FieldParticle *slots;
  u8 last;
  u32 i;

  Event_Start();
  Camera_MoveWithinMapBounds(-1, -1, -1, 0);
  BattleEffect_InitializeSharedScene();
  wait(10);
  Actor_ArmCallback(scene->firstObjectId, 0x4000, 0);
  wait(30);
  obj->update = Actor_ApplyRandomChildValues;
  Sound_PlayFar(131);
  Actor_SetMode(obj, 28);
  wait(40);

  Sound_PlayFar(220);
  Animation_ApplyChildValuesFar__launch(obj, 0);
  Actor_SetMode(obj, 3);
  obj->update = BattleEffect_AdvanceAngleWithRamp__launch;
  obj->timer = 0;
  wait(70);

  Actor_SetSingleChildFlagsFar__launch(obj, 0);
  obj->motionFlags = 0;
  obj->update = BattleEffect_UpdateDescendingObject__launch;
  obj->targetX = EFFECT_NO_TARGET;
  pos.x = obj->x;
  pos.y = obj->y;
  pos.z = obj->z;
  Camera_WorldToScreen__launch(&pos);
  i = 0;
  slot = scene->slots;
  slots = slot;
  do {
    s32 scale;

    FieldParticle_Initialize(slot, 0x2a0, pos.x, pos.z);
    FieldParticle_SetCallback(slot, Func_080e0cec);
    FieldParticle_SetActorMode(slot, 7);
    Animation_ApplyChildValuesToRecordFar(slot->object, (Random16() * 7) >> 16);
    scale = (Random16() >> 1) + 0x13333;
    slot->scaleY = scale;
    slot->scaleX = scale;
    wait(1);
    i++;
    slot++;
  } while (i < SLOT_MAX);
  wait(70);

  i = 0;
  last = 2;
  slot = slots;
  do {
    if (slot->active != 0)
      slot->state = last;
    i++;
    slot++;
  } while (i < SLOT_MAX);
  wait(40);
  BattleEffect_PrepareBufferInterpolation();
  obj->fall = 0;
  obj->y = obj->floor;
  wait(10);
}

void BattleEffect_UpdateDescendingParticlePositiveArc(struct Actor *particle) {
  struct Actor *source;
  s32 y;
  s32 limit;
  s32 scale;
  struct MainRoutineWork *party = &gMainRoutineWork;

  limit = particle->terrainHeight + 0xA0000;
  source = particle->watchedObject;
  if (party->map == (s32)&SceneId_Ov64a)
    limit = particle->terrainHeight + 0x40000;

  y = particle->y;
  if (y <= limit) {
    Actor_Destroy(particle);
    return;
  }

  scale = particle->scaleX + 0xC00;
  if (scale > Q16_ONE)
    scale = Q16_ONE;
  particle->scaleX = scale;
  particle->scaleY = scale;
  particle->x = source->x;
  particle->y = y - 0x20000;
  particle->z = source->z + ((0x10000 - scale) * 5) + 0x90000;
}

void BattleEffect_UpdateDescendingParticleNegativeArc(struct Actor *particle) {
  struct Actor *source;
  s32 y;
  s32 limit;
  s32 scale;
  struct MainRoutineWork *party = &gMainRoutineWork;

  limit = particle->terrainHeight + 0xA0000;
  source = particle->watchedObject;
  if (party->map == (s32)&SceneId_Ov64a)
    limit = particle->terrainHeight + 0x40000;

  y = particle->y;
  if (y <= limit) {
    Actor_Destroy(particle);
    return;
  }

  scale = particle->scaleX + 0xC00;
  if (scale > Q16_ONE)
    scale = Q16_ONE;
  particle->scaleX = scale;
  particle->scaleY = -scale;
  particle->x = source->x;
  particle->y = y - 0x20000;
  particle->z = source->z - ((0x10000 - scale) * 5) + 0x100000;
}

struct ArcObject *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
void Animation_ApplyChildArgumentFar(struct ArcSprite *sprite, s32 animation);

void Func_080e1024(struct ArcObject *source) {
  u8 *work = Ram_HeapSlots->effectWork;
  struct ArcObject *leader = *(struct ArcObject **)(work + 16);
  struct ArcObject *objects[2];
  struct ArcObject *object;
  struct ArcSprite *sprite;
  s32 i;
  u8 zero;

  for (i = 0; i <= 1; i++) {
    object = Func_080200c0(271, source->x, source->y + 0x400000, source->z);
    objects[i] = object;
    if (object == NULL)
      continue;
    object->unk14 = source->unk14;
    sprite = object->sprite;
    object->unk55 = 0;
    object->unk64 = 0;
    zero = 0;
    object->source = source;
    object->scaleY = 0x6666;
    object->scaleX = 0x6666;
    if (sprite == NULL)
      continue;
    Animation_ApplyChildArgumentFar(sprite, 0);
    sprite->animation = zero;
    Resource_ResetEntry(sprite->resource);
    sprite->resource = *(u16 *)(work + 0x710);
    sprite->active = 1;
    sprite->attr.half.tile =
        gResourceTableEntries[sprite->resource].offset >> 5;
    sprite->flags05_5 = 0;
    sprite->layer = 1;
    sprite->size = 2;
    sprite->picture->visible = zero;
  }
  objects[0]->callback =
      (void *)BattleEffect_UpdateDescendingParticleNegativeArc;
  objects[0]->sprite->attr.byte.variant = 0;
  objects[1]->callback =
      (void *)BattleEffect_UpdateDescendingParticlePositiveArc;
  objects[1]->sprite->attr.byte.variant = leader->sprite->attr.byte.variant;
}
