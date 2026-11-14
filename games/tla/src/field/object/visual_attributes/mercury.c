/// @file
/// A Mercury Djinni joining the party.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "gamepak/far_call/shop.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/field/field_sequence.h"
#include "module/field/object.h"
#include "module/system/system.h"
#include "scene_call.h"
#include "macros.h"

void Actor_MoveHalfwayTowardTargetCopy(struct Actor *object) {
  struct Actor *target = object->watchedObject;

  object->x += (target->x - object->x) / 2;
  object->y += (target->y - object->y) / 2;
  object->z += (target->z - object->z) / 2;
}

/* VIEW(f0d8740): tag struct CaptureObject as struct CaptureObject__f0d8740;
 * visual_attributes.c has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/object/visual_attributes/mercury.c:61: structure has no
 * member named `sprite'. */
struct CaptureObject__f0d8740 {
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
  struct CaptureObject__f0d8740 *linked;
  void *callback;
};

void Djinn_RunMercuryJoinEffect(s32 arg) {
  struct CaptureObject__f0d8740 *djinni;
  struct CaptureObject__f0d8740 *copy;
  struct CaptureObject__f0d8740 *previous;
  struct CaptureResource *resource;
  void *effectSlots;
  struct CaptureObject__f0d8740 **cursor;
  struct CaptureObject__f0d8740 **list;
  u8 *slot;
  s8 *slotState;
  s32 kind;
  s32 remaining;
  s32 resourceId;
  struct CaptureObject__f0d8740 *copies[8];
  struct {
    s32 x;
    s32 y;
    s32 z;
  } position;

  djinni = Actor_GetById(arg);
  if (djinni == NULL)
    return;

  FieldEffect_InitializeSlots();
  effectSlots = Ram_HeapSlots->effectWork;
  Shop_SaveAndTintFieldPaletteFar(0x204084);
  wait(30);
  djinni->visible = 0;
  Sound_PlayFar(SE_NOTE_LOW);
  Motion_SetVarCbAndRefresh(arg, 1);
  Sound_PlayFar(SE_NOTE_HIGH);
  Motion_SetVarCbAndRefresh(arg, 1);
  wait(20);
  Sound_PlayFar(SE_JUMP);
  Actor_Jump(arg, 3, 14);
  Sound_PlayFar(SE_JUMP);
  Actor_Jump(arg, 5, 16);
  Sound_PlayFar(SE_JUMP);
  Actor_Jump(arg, 7, 18);
  wait(20);

  kind = *djinni->sprite->resource;
  resource = NULL;
  previous = djinni;
  cursor = copies;
  for (remaining = 7; remaining >= 0; remaining--) {
    copy = Actor_CreateFar(kind, djinni->x, djinni->y, djinni->z);
    *cursor++ = copy;
    if (copy != NULL) {
      copy->scaleY = 0xf000;
      copy->scaleX = 0xf000;
      copy->mode = 0;
      copy->priorityFlags = 2;
      copy->collisionFlags |= 1;
      copy->callback = (void *)Actor_MoveHalfwayTowardTargetCopy;
      copy->facing = djinni->facing;
      Actor_SetSpriteColorModeFar((struct Actor *)copy, 9);
      Actor_SelectAnimationFar(copy, 0);
      Actor_SetSpriteFlagsFar((struct Actor *)copy, 0);
      resource = Sprite_ShareVramCacheEntry(copy->sprite, resource);
      copy->linked = previous;
      previous = copy;
    }
  }

  resourceId = resource->id;
  Sound_PlayFar(SE_HIGH_JUMP);
  djinni->mode = 0;
  for (remaining = 14; remaining >= 0; remaining--) {
    djinni->y += px(8);
    wait(1);
  }
  Actor_ReleaseFar(djinni);
  list = copies;
  for (remaining = 7; remaining >= 0; remaining--)
    Actor_ReleaseFar(*list++);
  if (resourceId != 96)
    VramCache_FreeEntry(resourceId);
  wait(10);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0x4000, 0);
  wait(20);
  Actor_SelectAnimationFar(Actor_GetById(gMainRoutineWork.currentOwner), 28);
  wait(20);

  SET_POSITION(&position, djinni->x, djinni->y, djinni->z);
  Camera_WorldToScreen(&position.x);

  slot = (u8 *)effectSlots + 80;
  remaining = 23;
  do {
    FieldParticle_Initialize(slot, 301, position.x, position.z);
    FieldParticle_SetCallback(slot,
                              (void *)FieldEffect_UpdateRandomTargetParticle);
    FieldParticle_SetActorMode(slot, 7);
    Animation_ApplyChildValuesToRecordFar(*(struct ActorSpriteView **)slot, 9);
    remaining--;
    wait(1);
    slot += 72;
  } while (remaining >= 0);

  wait(120);
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

  wait(50);
  Shop_RestoreFieldPaletteFar();
  FieldEffect_ReleaseSlots();
}
