// Near miss, differ 8 of 237 (2026-10-26): the twin of The Broken Seal's
// FieldEffect_RunMercuryDjinnCapture, with The Lost Age's effect work
// (Ram_HeapSlots->effectWork), leader (gPartyState.currentOwner), droplet
// object 301, sprite slots from 80 and the cache slot byte at 16. Only
// instruction order is left: after each store the native issues another
// instruction before the next load (the afterimage setup, the position copy).
#include "resource.h"
#include "types.h"
#include "object_dispatch.h"
#include "ram_buffer.h"
#include "party_state.h"


void BattleEffect_HalveDistanceToTarget(void);
void BattleEffect_UpdateRandomTargetParticle(void);

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
  u8 unk00[16];
  u8 id;
};


struct CaptureObject *Object_GetById(s32 id);
void BattleEffect_InitializeSlots(void);
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
void Shop_InitializeEffectFar(void);
void BattleEffect_ClearActiveSlotsAndScheduleUpdates(void);

void BattleEffect_RunMercuryDjinnCapture(s32 arg) {
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

  BattleEffect_InitializeSlots();
  effectSlots = Ram_HeapSlots->effectWork;
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
      copy->callback = (void *)BattleEffect_HalveDistanceToTarget;
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
  ObjectMotion_ArmCallback(gPartyState.currentOwner, 0x4000, 0);
  WaitFrames(20);
  Object_SetMode(Object_GetById(gPartyState.currentOwner), 28);
  WaitFrames(20);

  position.x = djinni->x;
  Camera_WorldToScreen(&position.x);
  position.y = djinni->y;
  position.z = djinni->z;

  slot = (u8 *)effectSlots + 80;
  remaining = 23;
  do {
    EffectSlot_Initialize(slot, 301, position.x, position.z);
    EffectSlot_SetCallback(slot, (void *)BattleEffect_UpdateRandomTargetParticle);
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
    slotState += 144;
    remaining = 23;
    do {
      if (slotState[5] != 0)
        slotState[0] = nextState;
      remaining--;
      slotState += 72;
    } while (remaining >= 0);
  }

  WaitFrames(50);
  Shop_InitializeEffectFar();
  BattleEffect_ClearActiveSlotsAndScheduleUpdates();
}
