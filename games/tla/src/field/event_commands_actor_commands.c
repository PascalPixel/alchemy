/// @file
/// Event commands on actors: their callbacks, sprite priority and still
/// pictures.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "fixed_math.h"
#include "field/field.h"
#include "system/system.h"

struct Actor *Actor_GetById(u32 objectId);

/// Clears an actor's destination, puts it under script control and
/// stands it still.
void Actor_ClearDestinationAndStand(u32 actorId) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->targetX = TARGET_UNSET;
    actor->targetY = TARGET_UNSET;
    actor->targetZ = TARGET_UNSET;
    Actor_StartIdleFar(actor);
    Actor_SelectAnimationFar(actor, ANIM_STAND);
  }
}

/// The event command "actor face direction": the actor turns toward angle
/// (TRIG_TURN a full turn; Actor_StepAngle), then the event waits the given
/// number of frames.
void Actor_FaceDirectionById(s32 actorId, s32 angle, s32 frames) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->timer = angle;
    Actor_StartScriptFar(actor, (const s32 *)gActor_StepAngleScript);
    Event_Wait(frames);
  }
}

void Actor_FaceDirectionAndRefreshById(s32 objectId, s32 angle) {
  struct Actor *object = Actor_Lookup(objectId);

  if (object != NULL) {
    object->timer = angle;
    Actor_StartScriptFar(object, (u32)gActor_StepAngleScript);
    Actor_WaitForScriptEndById(objectId);
  }
}

/// The event command "set actor sprite priority": fixes the priority of the
/// actor's sprite and its shadow, which then no longer follow its depth
/// (clears priorityFlags bit 0).
void Actor_SetSpritePriority(u32 objectId, s32 priority) {
  struct Actor *object = Actor_Lookup(objectId);

  if (object != NULL && (object->spriteStorageKind & 0xF) == 1) {
    struct Sprite *sprite = object->sprite;

    sprite->image.priority = priority;
    sprite->shadow.priority = priority;
    object->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  }
}

/// Makes the target actor's sprite show the source actor's graphics (its VRAM
/// block and tile). Neither actor is checked for NULL.
void Actor_CopyGraphics(u32 targetId, u32 sourceId) {
  struct Actor *actor;
  struct Sprite *sprite;
  u8 vramBlock;
  u32 tile;

  actor = Actor_GetById(sourceId);
  sprite = actor->sprite;
  vramBlock = sprite->slot;
  tile = sprite->image.tile;

  actor = Actor_GetById(targetId);
  sprite = actor->sprite;
  sprite->slot = vramBlock;
  sprite->image.tile = tile;
}

/// Stops actor id moving and colliding and hides it.
void Actor_DisableById(u32 id) {
  struct Actor *object = Actor_GetById(id);

  object->motionFlags = 0;
  object->collisionFlags = 0;
  Actor_SetSpriteFlagsFar(object, 0);
}

/// Turns actor id's sprite to rotation (1/65536 turns).
void Actor_SetSpriteRotationById(s32 id, s32 rotation) {
  struct Actor *object = Actor_GetById(id);
  struct Sprite *sprite = object->sprite;

  if (object != NULL && sprite != NULL)
    sprite->rotation = rotation;
}

/// Lays actor id down along angle, a quarter turn rounded: shows its first
/// frame turned, shifted by the side it falls to, and blocks its cell and the
/// cell beyond it.
void Actor_FallOverById(u32 id, u32 angle) {
  FIXED position[3];
  struct Actor *object = Actor_GetById(id);
  struct Sprite *sprite = object->sprite;

  if (object != NULL && sprite != NULL) {
    Actor_SelectAnimationFar(object, ANIM_STILL);
    angle = (angle + DEGtoANG(45)) & TRIG_TURN * 3 / 4;
    position[0] = object->x.fixed;
    position[2] = object->z.fixed;
    MapGrid_SetCellBehaviorAtFar(object->layer, position[0], position[2], 255);
    Vector_AddPolarOffset(toFIXED(16), angle, position);
    MapGrid_SetCellBehaviorAtFar(object->layer, position[0], position[2], 255);
    switch (angle) {
    case FACING_EAST:
      sprite->offsetX -= 3;
      sprite->offsetY += 5;
      break;
    case FACING_WEST:
      sprite->offsetX += 2;
      sprite->offsetY += 4;
      break;
    case FACING_NORTH:
      sprite->offsetY += 1;
      break;
    case FACING_SOUTH:
      sprite->offsetY += 5;
      break;
    }
    object->collisionFlags = 0;
    object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    sprite->flags = 0;
    sprite->rotation = angle + DEGtoANG(90);
  }
}

/// Actor_FallOverById with an animation (mode, -1 for none) and the fall
/// moving the actor itself half a cell the way it falls.
void Actor_FallOverWithAnimationById(s32 id, u32 angle, s32 mode) {
  struct Actor *object = Actor_GetById(id);
  struct Sprite *sprite = object->sprite;
  FIXED position[3];

  if (object == NULL || sprite == NULL)
    return;
  if (mode != -1)
    Actor_SelectAnimationFar(object, mode);
  position[0] = object->x.fixed;
  angle = (angle + DEGtoANG(45)) & TRIG_TURN * 3 / 4;
  position[2] = object->z.fixed;
  MapGrid_SetCellBehaviorAtFar(object->layer, position[0], position[2], 255);
  Vector_AddPolarOffset(toFIXED(16), angle, position);
  MapGrid_SetCellBehaviorAtFar(object->layer, position[0], position[2], 255);
  switch (angle) {
  case 0:
    object->x.fixed += toFIXED(8);
    object->z.fixed += toFIXED(1);
    sprite->offsetX -= 4;
    sprite->offsetY += 15;
    break;
  case 0x8000:
    object->x.fixed -= toFIXED(8);
    object->z.fixed += toFIXED(1);
    sprite->offsetX += 3;
    sprite->offsetY += 16;
    break;
  case 0xc000:
    sprite->offsetY += 1;
    break;
  case 0x4000:
    object->z.fixed += toFIXED(8);
    sprite->offsetY += 13;
    break;
  }
  object->collisionFlags = 0;
  object->priorityFlags |= 2;
  sprite->flags = 0;
  sprite->rotation = angle + DEGtoANG(90);
}

/// Gives an actor's sprite a still picture: resource, drawn by Icon_DrawItemFar
/// into a cleared 128-byte buffer, or a blank one when bit 15 is set, loaded
/// into the sprite's VRAM block as its tile.
void Actor_ShowItemIconById(s32 id, s32 resource) {
  struct Actor *object = Actor_GetById(id);
  struct Sprite *sprite;
  u8 *buffer;
  s32 zero;
  s32 tile;
  u8 kind;

  if (object != NULL) {
    kind = object->spriteStorageKind;
    if (kind == 1) {
      sprite = object->sprite;
      if (resource & 0x8000)
        resource = 0;
      buffer = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
      zero = 0;
      buffer += 0x400;
      Dma_Fill32(zero, buffer, 0x80);
#ifdef UBFIX
      Icon_DrawItemFar(resource);
#else
      // UB: Icon_DrawItem takes the item alone; the call passes the buffer too.
      ((void (*)(s32, u8 *))Icon_DrawItemFar)(resource, buffer);
#endif
      tile = VramCache_Load(sprite->slot, 0x80, buffer);
      HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
      object->retainSpriteCache = kind;
      SpriteAnimation_ClearFar(sprite->entries[0]);
      sprite->entries[0] = NULL;
      sprite->count = zero;
      sprite->image.fullColor = zero;
      sprite->image.tile = tile;
      sprite->dirty = zero;
      sprite->flags = zero;
    }
  }
}

/// The event command "set message": the number of the event's next message,
/// written to work->message.
void Event_SetMessageId(s32 message) { gFieldWork->message = message; }

/// Returns the sprite (the first animation's id) of the actor in the low 12
/// bits of actorId, or -1 if there is no such animated actor.
s32 Actor_GetSpriteById(s32 key) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  s32 result = -1;
  struct Actor *object = event->actors[key & 0x0fff];

  if (object != NULL && object->spriteStorageKind == 1)
    result = ((struct Sprite *)object->sprite)->entries[0]->animationId;
  return result;
}

/// Returns the first of the scene's placed actors (ids 8 to 79) showing
/// sprite, or -1.
s32 Actor_FindBySprite(s32 value) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  s32 result = -1;
  s32 index;
  struct Actor *object;

  for (index = 8; index <= 79; index++) {
    object = event->actors[index];
    if (object != NULL && object->spriteStorageKind == 1 &&
        ((struct Sprite *)object->sprite)->entries[0]->animationId == value) {
      result = index;
      break;
    }
  }
  return result;
}
