/// @file
/// Event commands on actors: their callbacks, sprite priority and still
/// pictures.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "fixed_math.h"
#include "field/field.h"
#include "field/speaker.h"
#include "graphics/glyph.h"
#include "item_ids.h"
#include "system/system.h"

struct Actor *Actor_GetById(u32 objectId);

/// Clears an actor's destination, starts its idle script and stands it
/// still.
void Actor_ClearDestinationAndStand(u32 actorId) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->targetX = ACTOR_NO_TARGET;
    actor->targetY = ACTOR_NO_TARGET;
    actor->targetZ = ACTOR_NO_TARGET;
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

/// Turns an actor a step toward angle (gActor_StepAngleScript) and waits for
/// the script to end.
void Actor_FaceDirectionAndRefreshById(s32 actorId, s32 angle) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->timer = angle;
    Actor_StartScriptFar(actor, (const s32 *)gActor_StepAngleScript);
    Actor_WaitForScriptEndById(actorId);
  }
}

/// The event command "set actor sprite priority": fixes the priority of the
/// actor's sprite and its shadow, which then no longer follow its depth:
/// clears ACTOR_PRIORITY_AUTOMATIC.
void Actor_SetSpritePriority(u32 actorId, s32 priority) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL &&
      (actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE) {
    struct Sprite *sprite = actor->sprite;

    sprite->image.priority = priority;
    sprite->shadow.priority = priority;
    actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
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
  struct Actor *actor = Actor_GetById(id);

  actor->motionFlags = 0;
  actor->collisionFlags = 0;
  Actor_SetSpriteFlagsFar(actor, 0);
}

/// Turns actor id's sprite to rotation (1/65536 turns).
void Actor_SetSpriteRotationById(s32 id, s32 rotation) {
  struct Actor *actor = Actor_GetById(id);
  struct Sprite *sprite = actor->sprite;

  if (actor != NULL && sprite != NULL)
    sprite->rotation = rotation;
}

/// Lays actor id down along angle, a quarter turn rounded: shows its first
/// frame turned, shifted by the side it falls to, and blocks its cell and the
/// cell beyond it.
void Actor_FallOverById(u32 id, u32 angle) {
  FIXED position[3];
  struct Actor *actor = Actor_GetById(id);
  struct Sprite *sprite = actor->sprite;

  if (actor != NULL && sprite != NULL) {
    Actor_SelectAnimationFar(actor, ANIM_STILL);
    angle = (angle + DEGtoANG(45)) & TRIG_TURN * 3 / 4;
    position[0] = actor->x.fixed;
    position[2] = actor->z.fixed;
    MapGrid_SetCellBehaviorAtFar(actor->layer, position[0], position[2],
                                 CELL_BEHAVIOR_WALL);
    Vector_AddPolarOffset(toFIXED(16), angle, position);
    MapGrid_SetCellBehaviorAtFar(actor->layer, position[0], position[2],
                                 CELL_BEHAVIOR_WALL);
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
    actor->collisionFlags = 0;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    sprite->flags = 0;
    sprite->rotation = angle + DEGtoANG(90);
  }
}

/// Actor_FallOverById with an animation of the actor's sprite sheet (-1 for
/// none) and the fall moving the actor itself half a cell the way it falls.
void Actor_FallOverWithAnimationById(s32 id, u32 angle, s32 animation) {
  struct Actor *actor = Actor_GetById(id);
  struct Sprite *sprite = actor->sprite;
  FIXED position[3];

  if (actor == NULL || sprite == NULL)
    return;
  if (animation != -1)
    Actor_SelectAnimationFar(actor, animation);
  position[0] = actor->x.fixed;
  angle = (angle + DEGtoANG(45)) & TRIG_TURN * 3 / 4;
  position[2] = actor->z.fixed;
  MapGrid_SetCellBehaviorAtFar(actor->layer, position[0], position[2],
                               CELL_BEHAVIOR_WALL);
  Vector_AddPolarOffset(toFIXED(16), angle, position);
  MapGrid_SetCellBehaviorAtFar(actor->layer, position[0], position[2],
                               CELL_BEHAVIOR_WALL);
  switch (angle) {
  case FACING_EAST:
    actor->x.fixed += toFIXED(8);
    actor->z.fixed += toFIXED(1);
    sprite->offsetX -= 4;
    sprite->offsetY += 15;
    break;
  case FACING_WEST:
    actor->x.fixed -= toFIXED(8);
    actor->z.fixed += toFIXED(1);
    sprite->offsetX += 3;
    sprite->offsetY += 16;
    break;
  case FACING_NORTH:
    sprite->offsetY += 1;
    break;
  case FACING_SOUTH:
    actor->z.fixed += toFIXED(8);
    sprite->offsetY += 13;
    break;
  }
  actor->collisionFlags = 0;
  actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  sprite->flags = 0;
  sprite->rotation = angle + DEGtoANG(90);
}

/// Item 0's icon, a gold coin, which an actor shows for coins.
#define COIN_ICON_ITEM 0

/// Shows an actor as an item's icon, its sprite's only picture: the item, or
/// for coins (SCOOPED_COINS) COIN_ICON_ITEM's.
void Actor_ShowItemIconById(s32 id, s32 item) {
  struct Actor *actor = Actor_GetById(id);
  struct Sprite *sprite;
  IconTransfer *icon;
  s32 tile;
  u8 kind;

  if (actor != NULL) {
    kind = actor->spriteStorageKind;
    if (kind == ACTOR_SPRITE_ONE) {
      sprite = actor->sprite;
      if (item & SCOOPED_COINS)
        item = COIN_ICON_ITEM;
      icon = HeapSlot_Alloc(HEAP_SLOT_ICON_TRANSFER, sizeof(IconTransfer));
      Dma_Fill32(0, icon->tiles, ICON_TILE_BYTES);
#ifdef UBFIX
      Icon_DrawItemFar(item);
#else
      // UB: Icon_DrawItem takes the item alone; the call passes the tiles too.
      ((void (*)(s32, u8 *))Icon_DrawItemFar)(item, icon->tiles);
#endif
      tile = VramCache_Load(sprite->slot, ICON_TILE_BYTES, icon->tiles);
      HeapSlot_Release(HEAP_SLOT_ICON_TRANSFER);
      actor->retainSpriteCache = kind;
      SpriteAnimation_ClearFar(sprite->entries[0]);
      sprite->entries[0] = NULL;
      sprite->count = 0;
      sprite->image.fullColor = 0;
      sprite->image.tile = tile;
      sprite->dirty = 0;
      sprite->flags = 0;
    }
  }
}

/// The event command "set message": the number of the event's next message,
/// written to the field state's message.
void Event_SetMessageId(s32 message) { gFieldState->message = message; }

/// Returns the sprite (the first animation's id) of the actor whose id is
/// key's under SPEAKER_ACTOR_MASK, or -1 if there is no such animated actor.
s32 Actor_GetSpriteById(s32 key) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  s32 result = -1;
  struct Actor *actor = state->actors[key & SPEAKER_ACTOR_MASK];

  if (actor != NULL && actor->spriteStorageKind == ACTOR_SPRITE_ONE)
    result = ((struct Sprite *)actor->sprite)->entries[0]->animationId;
  return result;
}

/// Returns the first of the scene's placed actors (from ACTOR_FIRST_PLACED)
/// showing spriteId, or -1.
s32 Actor_FindBySprite(s32 spriteId) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  s32 result = -1;
  s32 index;
  struct Actor *actor;

  for (index = ACTOR_FIRST_PLACED; index < FIELD_ACTOR_COUNT; index++) {
    actor = state->actors[index];
    if (actor != NULL && actor->spriteStorageKind == ACTOR_SPRITE_ONE &&
        ((struct Sprite *)actor->sprite)->entries[0]->animationId == spriteId) {
      result = index;
      break;
    }
  }
  return result;
}
