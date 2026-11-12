/// @file
/// Routines whose twins are in The Broken Seal's
/// field/event_commands/retreat.c.
// Part of radial.c.

#include "../radial_internal.h"

#define SLOT_MAX 24

struct SpinObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  s32 floor;
  s32 fall;
  u8 unk1c[0x1c];
  s32 targetX;
  u8 unk3c[0x19];
  u8 motionFlags;
  u8 unk56[0xe];
  s16 timer;
  u8 unk66[6];
  void (*update)(struct SpinObject *);
};

struct SpinScene {
  u8 unk00[0x10];
  struct SpinObject *mainObject;
  u8 unk14[4];
  s16 firstObjectId;
  u8 unk1a[0x36];
  struct FieldParticle slots[SLOT_MAX];
};

struct SpinVector {
  s32 x;
  s32 y;
  s32 z;
};

void Event_Start(void);
void Camera_MoveWithinMapBounds(s32, s32, s32, s32);
void BattleEffect_InitializeSharedScene(void);
void wait(s32 frames);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Sound_PlayFar(s32 cue);
void Camera_WorldToScreen(struct EffectVector *position);
void FieldParticle_Initialize(struct FieldParticle *slot, s32 kind, s32 x,
                              s32 z);
u32 Random16(void);
void BattleEffect_PrepareBufferInterpolation(void);
void Actor_ApplyRandomChildValues(struct SpinObject *owner);
void Func_080e0cec(struct FieldParticle *slot);

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
  Animation_ApplyChildValuesFar(obj, 0);
  Actor_SetMode(obj, 3);
  obj->update = BattleEffect_AdvanceAngleWithRamp;
  obj->timer = 0;
  wait(70);

  Actor_SetSingleChildFlagsFar(obj, 0);
  obj->motionFlags = 0;
  obj->update = BattleEffect_UpdateDescendingObject;
  obj->targetX = EFFECT_NO_TARGET;
  pos.x = obj->x;
  pos.y = obj->y;
  pos.z = obj->z;
  Camera_WorldToScreen(&pos);
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


void Actor_Destroy(struct Actor *object);

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


struct ArcPicture {
  u8 padding00[22];
  u8 visible;
};

struct ArcSprite {
  u8 padding00[5];
  u8 flags05_0 : 5;
  u8 flags05_5 : 1;
  u8 layer : 2;
  u8 padding06;
  u8 flags07_0 : 6;
  u8 size : 2;
  union {
    struct {
      u16 tile : 10;
      u16 flags08_10 : 6;
    } half;
    struct {
      u8 tileLow;
      u8 flags09_0 : 2;
      u8 variant : 2;
      u8 flags09_4 : 4;
    } byte;
  } attr;
  u8 padding0c[4];
  u8 resource;
  u8 active : 1;
  u8 flags11_1 : 7;
  u8 padding12[8];
  u8 animation;
  u8 padding1b[13];
  struct ArcPicture *picture;
};

struct ArcObject {
  u8 padding00[8];
  s32 x;
  s32 y;
  s32 z;
  s32 unk14;
  s32 scaleX;
  s32 scaleY;
  u8 padding20[0x30];
  struct ArcSprite *sprite;
  u8 padding54;
  u8 unk55;
  u8 padding56[0x0e];
  u16 unk64;
  u8 padding66[2];
  struct ArcObject *source;
  void *callback;
};


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
