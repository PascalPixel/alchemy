#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "event_work.h"
#include "object_runtime.h"
#include "objdisp.h"
#include "field_sprite.h"
#include "edition.h"

struct ObjectRuntime *Object_GetById(u32);

void Object_ResetTargetAndSetMode1(u32 objectId) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    Object_ResetMotion(object);
    Object_SetMode(object, 1);
  }
}

void ObjectMotion_ArmCallback(s32 objectId, s32 angle, s32 wait) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->action = angle;
    ObjectDispatch_InitializeFar((struct DispatchObject *)object,
                                 (u32)gObjectMotion_StepAngleScript);
    EventRuntime_Wait(wait);
  }
}

void ObjectMotion_ArmCallbackAndRefresh(s32 objectId, s32 angle) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->action = angle;
    ObjectDispatch_InitializeFar((struct DispatchObject *)object,
                                 (u32)gObjectMotion_StepAngleScript);
    Object_RefreshSelectorById(objectId);
  }
}

void ObjectMotion_SetActionVariant(u32 objectId, s32 priority) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL && (object->animationKind & 0xF) == 1) {
    struct FieldSprite *sprite = object->animation;

    sprite->priority = priority;
    sprite->partPriority = priority;
    object->priorityFlags &= ~1;
  }
}

void ObjectVisual_CopyAttributes(u32 targetId, u32 sourceId) {
  struct FieldSprite *sprite;
  u8 block;
  u32 tile;

  sprite = (struct FieldSprite *)Object_GetById(sourceId);
  sprite = ((struct ObjectRuntime *)sprite)->animation;
  block = sprite->vramBlock;
  tile = sprite->tile;

  sprite = (struct FieldSprite *)Object_GetById(targetId);
  sprite = ((struct ObjectRuntime *)sprite)->animation;
  sprite->vramBlock = block;
  sprite->tile = tile;
}

void Func_080d390c(u32 id) {
  struct ObjectRuntime *object = Object_GetById(id);

  object->motionFlags = 0;
  object->collisionFlags = 0;
  ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)object, 0);
}

void Func_080d3928(u32 id, s32 rotation) {
  struct ObjectRuntime *object = Object_GetById(id);
  struct FieldSprite *sprite = object->animation;

  if (object != NULL && sprite != NULL)
    sprite->rotation = rotation;
}
