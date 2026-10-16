#include "resource.h"
#include "types.h"
#include "object_dispatch.h"

extern u8 gEffectWork[];

void BattleFx_HalveDistanceToTarget(void);
void BattleFx_UpdateRandomTargetParticle(void);

struct CaptureSprite {
  u8 unk00[40];
  s16 *resource;
};

struct CaptureObject {
  u8 unk00[6];
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[4];
  s32 scaleX;
  s32 scaleY;
  u8 unk20[3];
  u8 priorityFlags;
  u8 unk24[44];
  struct CaptureSprite *sprite;
  u8 unk54;
  u8 mode;
  u8 unk56[4];
  u8 collisionFlags;
  u8 visible;
  u8 unk5c[12];
  struct CaptureObject *linked;
  void *callback;
};

struct CaptureResource {
  u8 unk00[28];
  u8 id;
};

extern s32 gGameState[];

struct CaptureObject *Object_GetById(s32 id);
void BattleFx_InitializeSlots(void);
void Shop_RestoreSceneTilesFar(s32 value);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void Motion_SetVarCbAndRefresh(s32 id, s32 value);
void ObjectMotion_Launch(s32 id, s32 height, s32 frames);
struct CaptureObject *Object_CreateFar(s32 kind, s32 x, s32 y, s32 z);
void Object_SetMode(struct CaptureObject *object, s32 mode);
struct CaptureResource *
Object_ReplaceResourceEntry(struct CaptureSprite *sprite,
                            struct CaptureResource *resource);
void Func_080090d0(struct CaptureObject *object);
void ObjectMotion_ArmCallback(s32 id, s32 value, s32 flags);
void Camera_WorldToScreen(s32 *position);
void EffectSlot_Initialize(void *slot, s32 kind, s32 x, s32 y);
void EffectSlot_SetCallback(void *slot, void *callback);
void EffectSlot_SetObjectMode(void *slot, s32 mode);
void Shop_InitEffectFar(void);
void BattleFx_ClearActiveSlotsAndScheduleUpdates(void);

void BattleFx_RunMercuryDjinnCapture(s32 arg) {
  struct CaptureObject *djinni;
  struct CaptureObject *copy;
  struct CaptureObject *previous;
  struct CaptureResource *resource;
  void *effectSlots;
  struct CaptureObject **cursor;
  struct CaptureObject **list;
  u8 *slot;
  s8 *slotState;
  s32 kind;
  s32 remaining;
  s32 resourceId;
  struct CaptureObject *copies[8];
  struct {
    s32 x;
    s32 y;
    s32 z;
  } position;

  djinni = Object_GetById(arg);
  if (djinni == NULL)
    return;

  BattleFx_InitializeSlots();
  effectSlots = *(void **)gEffectWork;
  Shop_RestoreSceneTilesFar(0x204084);
  WaitFrames(30);
  djinni->visible = 0;
  Audio_PlayCue(173);
  Motion_SetVarCbAndRefresh(arg, 1);
  Audio_PlayCue(175);
  Motion_SetVarCbAndRefresh(arg, 1);
  WaitFrames(20);
  Audio_PlayCue(152);
  ObjectMotion_Launch(arg, 3, 14);
  Audio_PlayCue(152);
  ObjectMotion_Launch(arg, 5, 16);
  Audio_PlayCue(152);
  ObjectMotion_Launch(arg, 7, 18);
  WaitFrames(20);

  kind = *djinni->sprite->resource;
  resource = NULL;
  previous = djinni;
  cursor = copies;
  for (remaining = 7; remaining >= 0; remaining--) {
    copy = Object_CreateFar(kind, djinni->x, djinni->y, djinni->z);
    *cursor++ = copy;
    if (copy != NULL) {
      copy->scaleY = 0xf000;
      copy->scaleX = 0xf000;
      copy->mode = 0;
      copy->priorityFlags = 2;
      copy->collisionFlags |= 1;
      copy->callback = (void *)BattleFx_HalveDistanceToTarget;
      copy->facing = djinni->facing;
      Animation_ApplyChildValuesFar((struct DispatchObject *)copy, 9);
      Object_SetMode(copy, 0);
      ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)copy, 0);
      resource = Object_ReplaceResourceEntry(copy->sprite, resource);
      copy->linked = previous;
      previous = copy;
    }
  }

  resourceId = resource->id;
  Audio_PlayCue(153);
  djinni->mode = 0;
  for (remaining = 14; remaining >= 0; remaining--) {
    djinni->y += 0x80000;
    WaitFrames(1);
  }
  Func_080090d0(djinni);
  list = copies;
  for (remaining = 7; remaining >= 0; remaining--)
    Func_080090d0(*list++);
  if (resourceId != 96)
    Resource_ResetEntry(resourceId);
  WaitFrames(10);
  ObjectMotion_ArmCallback(gGameState[125], 0x4000, 0);
  WaitFrames(20);
  Object_SetMode(Object_GetById(gGameState[125]), 28);
  WaitFrames(20);

  position.x = djinni->x;
  position.y = djinni->y;
  position.z = djinni->z;
  Camera_WorldToScreen(&position.x);

  slot = (u8 *)effectSlots + 88;
  remaining = 23;
  do {
    EffectSlot_Initialize(slot, 240, position.x, position.z);
    EffectSlot_SetCallback(slot, (void *)BattleFx_UpdateRandomTargetParticle);
    EffectSlot_SetObjectMode(slot, 7);
    Animation_ApplyChildValuesToRecordFar(*(struct DispatchChild **)slot, 9);
    remaining--;
    WaitFrames(1);
    slot += 72;
  } while (remaining >= 0);

  WaitFrames(120);
  {
    s32 nextState;

    nextState = 2;
    slotState = effectSlots;
    slotState += 152;
    remaining = 23;
    do {
      if (slotState[5] != 0)
        slotState[0] = nextState;
      remaining--;
      slotState += 72;
    } while (remaining >= 0);
  }

  WaitFrames(50);
  Shop_InitEffectFar();
  BattleFx_ClearActiveSlotsAndScheduleUpdates();
}
