/// @file
/// Event commands on actors: their callbacks, sprite priority and still
/// pictures.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"
#include "fixed_math.h"
#include "module/field/field.h"
#include "module/system/system.h"

struct Actor *Actor_GetById(u32 objectId);

void Actor_ResetTargetAndSetMode1(u32 objectId) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    Actor_StartIdleFar(object);
    Actor_SelectAnimationFar(object, 1);
  }
}

void Actor_ArmCallback(s32 objectId, s32 angle, s32 frames) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->timer = angle;
    Actor_StartScriptFar(object, (u32)gActor_StepAngleScript);
    Event_Wait(frames);
  }
}

void Actor_FaceDirectionById(s32 objectId, s32 angle) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->timer = angle;
    Actor_StartScriptFar(object, (u32)gActor_StepAngleScript);
    Actor_RefreshSelectorById(objectId);
  }
}

void Actor_SetSpritePriority(u32 objectId, s32 priority) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL && (object->animationKind & 0xF) == 1) {
    struct FieldSprite *sprite = object->sprite;

    sprite->priority = priority;
    sprite->partPriority = priority;
    object->priorityFlags &= ~1;
  }
}

void Actor_CopyGraphics(u32 targetId, u32 sourceId) {
  struct FieldSprite *sprite;
  u8 block;
  u32 tile;

  sprite = (struct FieldSprite *)Actor_GetById(sourceId);
  sprite = ((struct Actor *)sprite)->sprite;
  block = sprite->vramBlock;
  tile = sprite->tile;

  sprite = (struct FieldSprite *)Actor_GetById(targetId);
  sprite = ((struct Actor *)sprite)->sprite;
  sprite->vramBlock = block;
  sprite->tile = tile;
}

/// Stops actor id moving and colliding and hides it.
void Actor_DisableById(u32 id) {
  struct Actor *object = Actor_GetById(id);

  object->motionFlags = 0;
  object->collisionFlags = 0;
  Actor_SetSpriteFlagsFar(object, 0);
}

void Actor_SetSpriteRotationById(s32 id, s32 rotation) {
  struct Actor *object = Actor_GetById(id);
  struct FieldSprite *sprite = object->sprite;

  if (object != NULL && sprite != NULL)
    sprite->rotation = rotation;
}

/* VIEW(visual_attributes): routine Icon_DrawItemFar as void
 * Icon_DrawItemFar(s32 resource, u8 *buffer); module/field/field.h has void
 * Icon_DrawItemFar(s32 item); tried as the rest of the object declares it, it
 * does not compile in tla-ja:
 * games/tla/src/field/object/visual_attributes/actor_commands.c:219: too many
 * arguments to function `Icon_DrawItemFar'. */
extern void
Func_08038248__visual_attributes(s32 resource,
                                 u8 *buffer) __asm__("Icon_DrawItemFar");

/// Lays actor id down along angle, a quarter turn rounded: shows its first
/// frame turned, shifted by the side it falls to, and blocks its cell and the
/// cell beyond it.
void Actor_FallOverById(u32 id, u32 angle) {
  s32 position[3];
  struct Actor *object = Actor_GetById(id);
  struct ObjectSprite *sprite = object->sprite;

  if (object != NULL && sprite != NULL) {
    Actor_SelectAnimationFar(object, 0);
    angle = (angle + TRIG_TURN / 8) & TRIG_TURN * 3 / 4;
    position[0] = object->x;
    position[2] = object->z;
    MapGrid_SetCellBehaviorAtFar(object->terrainId, position[0], position[2],
                                 255);
    Vector_AddPolarOffset(px(16), angle, position);
    MapGrid_SetCellBehaviorAtFar(object->terrainId, position[0], position[2],
                                 255);
    switch (angle) {
    case 0:
      sprite->offsetX -= 3;
      sprite->offsetY += 5;
      break;
    case 0x8000:
      sprite->offsetX += 2;
      sprite->offsetY += 4;
      break;
    case 0xc000:
      sprite->offsetY += 1;
      break;
    case 0x4000:
      sprite->offsetY += 5;
      break;
    }
    object->collisionFlags = 0;
    object->priorityFlags |= 2;
    sprite->nextFrame = 0;
    sprite->rotation = angle + TRIG_TURN / 4;
  }
}

/// Actor_FallOverById with an animation (mode, -1 for none) and the fall
/// moving the actor itself half a cell the way it falls.
void Actor_FallOverWithAnimationById(s32 id, u32 angle, s32 mode) {
  struct Actor *object = Actor_GetById(id);
  struct FieldSprite *sprite = object->sprite;
  s32 position[3];

  if (object == NULL || sprite == NULL)
    return;
  if (mode != -1)
    Actor_SelectAnimationFar(object, mode);
  position[0] = object->x;
  angle = (angle + TRIG_TURN / 8) & TRIG_TURN * 3 / 4;
  position[2] = object->z;
  MapGrid_SetCellBehaviorAtFar(object->terrainId, position[0], position[2],
                               255);
  Vector_AddPolarOffset(px(16), angle, position);
  MapGrid_SetCellBehaviorAtFar(object->terrainId, position[0], position[2],
                               255);
  switch (angle) {
  case 0:
    object->x += px(8);
    object->z += px(1);
    ((s8 *)sprite->unk14)[2] -= 4;
    ((s8 *)sprite->unk14)[3] += 15;
    break;
  case 0x8000:
    object->x -= px(8);
    object->z += px(1);
    ((s8 *)sprite->unk14)[2] += 3;
    ((s8 *)sprite->unk14)[3] += 16;
    break;
  case 0xc000:
    ((s8 *)sprite->unk14)[3] += 1;
    break;
  case 0x4000:
    object->z += px(8);
    ((s8 *)sprite->unk14)[3] += 13;
    break;
  }
  object->collisionFlags = 0;
  object->priorityFlags |= 2;
  sprite->nextFrame = 0;
  sprite->rotation = angle + TRIG_TURN / 4;
}

/// Gives an actor's sprite a still picture: resource, drawn by Icon_DrawItemFar
/// into a cleared 128-byte buffer, or a blank one when bit 15 is set, loaded
/// into the sprite's VRAM block as its tile.
void Actor_ShowItemIconById(s32 id, s32 resource) {
  struct Actor *object = Actor_GetById(id);
  struct FieldSprite *sprite;
  u8 *buffer;
  s32 zero;
  s32 tile;
  u8 kind;

  if (object != NULL) {
    kind = object->animationKind;
    if (kind == 1) {
      sprite = object->sprite;
      if (resource & 0x8000)
        resource = 0;
      buffer = HeapSlot_Alloc(68, 0x608);
      zero = 0;
      buffer += 0x400;
      Dma_Fill32(zero, buffer, 0x80);
      Func_08038248__visual_attributes(resource, buffer);
      tile = VramCache_Load(sprite->vramBlock, 0x80, buffer);
      HeapSlot_Release(68);
      object->unk5c[0] = kind;
      SpriteAnimation_ClearFar(sprite->record);
      sprite->record = NULL;
      sprite->unk1b[0] = zero;
      sprite->fullColor = zero;
      sprite->tile = tile;
      sprite->unk19 = zero;
      sprite->nextFrame = zero;
    }
  }
}

void Event_SetMessageId(s32 message) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;

  event->message = message;
}

s32 Actor_GetSpriteById(s32 key) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  s32 result = -1;
  struct Actor *object = event->actors[key & 0x0fff];

  if (object != NULL && object->animationKind == 1)
    result = *((struct ObjectAnimationState *)object->sprite)->value28;
  return result;
}

s32 Actor_FindBySprite(s32 value) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  s32 result = -1;
  s32 index;
  struct Actor *object;

  for (index = 8; index <= 79; index++) {
    object = event->actors[index];
    if (object != NULL && object->animationKind == 1 &&
        *((struct ObjectAnimationState *)object->sprite)->value28 == value) {
      result = index;
      break;
    }
  }
  return result;
}
