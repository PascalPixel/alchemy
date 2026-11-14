/// @file
/// Moving actors by the direction pad, and the party's keys.
// Part of field.c.

#include "../field_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "gamepak/far_call/field_sequence.h"
#include "module/field/field.h"
#include "module/field/resource_slot.h"
#include "macros.h"

/// The heading of each direction-pad combination, 0xffff for none.

void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z);

/// The script command 0x1b: moves an object 8 pixels at a time by the
/// direction pad, with up and down changing its height (y) rather than its
/// depth. Refuses steps into an occupied cell, from or onto a cell without
/// attribute bit 6, or onto ground too high (up) or too low (down), and
/// records the held keys in the event work. Returns 1.
// As ☀️'s Actor_MoveByKeys.
s32 Actor_MoveByKeys(struct Actor *object) {
  struct FieldPosition position;
  struct MapCell *sourceCell;
  struct MapCell *destinationCell;
  struct FieldWork *work;
  u16 angle;
  s32 animation;
  s32 collisionFlags;
  s32 deltaZ;
  s32 terrainHeight;
  struct MapWork *map;
  u32 keys;
  s32 half;

  object->acceleration = px(1) / 4;
  animation = 12;
  half = 0x8000;
  object->speedLimit = half;
  if (gDebugMode)
    keys = gInput.held;
  keys = gInput.held;
  {
    const u16 *table = gWorldMap_KeyHeadings;

    angle = *(const u16 *)((const u8 *)table + (((keys >> 4) & 15) << 1));
  }
  if (angle == 0xffff) {
    collisionFlags = 4;
  } else {
    animation = 14;
    if ((angle & 0xf000) != 0) {
      animation = 15;
      if ((angle & 0xf000) != half)
        animation = 10;
    }
    collisionFlags = 0;
    SET_POSITION(&position, 0, 0, 0);
    Vector_AddPolarOffset(px(8), angle, &position);
    position.x += object->x;
    deltaZ = position.z;
    if (deltaZ < 0)
      object->facing = TRIG_TURN * 3 / 4;
    if (deltaZ > 0)
      object->facing = TRIG_TURN / 4;
    position.y = object->y - position.z;
    position.z = object->z;
    map = Ram_HeapSlots->mapWork;
    sourceCell =
        &map->layers[object->terrainId].cells[Actor_GetCellIndexFar(object)];
    destinationCell =
        &map->layers[object->terrainId]
             .cells[position.x / 0x100000 + (position.z / 0x100000) * 128];
    if (MapGrid_IsWallAt((s32 *)&position) != 0 ||
        !(sourceCell->attributeB & 64) || !(destinationCell->attributeB & 64)) {
      collisionFlags = 4;
      animation = 12;
    } else if (gInput.held & 64) {
      terrainHeight = MapGrid_GetTerrainHeight(object->terrainId, position.x,
                                               position.z - 0x100000);
      if (terrainHeight - object->y < 0x100000)
        goto refused;
    } else if (gInput.held & 128) {
      terrainHeight =
          MapGrid_GetTerrainHeight(object->terrainId, position.x, position.z);
      if (terrainHeight - object->y > -0x80000) {
      refused:
        collisionFlags = 1;
        animation = 12;
      }
    }
  }
  work = Ram_HeapSlots->fieldWork;
  if (work != NULL) {
    if (collisionFlags & 3)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = gInput.held;
  }
  Actor_SelectAnimation(object, animation);
  if (collisionFlags != 0) {
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    object->velocityX = 0;
    object->velocityY = 0;
    object->velocityZ = 0;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
  }
  object->step++;
  return 1;
}

/// Script command 0x1c, the leader's control on a tied rope (render mode 3):
/// the pad moves it from node to node along the rope, a half cell a step.
s32 Actor_CrossRope(struct Actor *object) {
  struct FieldPosition position;
  s32 collisionFlags;
  struct SpriteAngle *sprite;
  struct RopeWork *rope;
  s32 bestDistance;
  s32 value;
  s32 speed;
  struct RopeNode *best;
  struct RopeNode *node;
  struct RopeSlot *slot;
  s32 count;
  u16 angle;
  s32 turn;
  struct Actor *first;
  struct Actor *second;
  struct FieldWork *work;

  sprite = object->sprite;
  rope = *(struct RopeWork **)&Ram_HeapSlots->unk84[6];
  speed = 0x10000;
  collisionFlags = 0;
  object->speedLimit = px(1) / 2;
  object->acceleration = px(1) / 2;
  object->motionFlags = 0;
  angle = gWorldMap_KeyHeadings[(gInput.held >> 4) & 15];
  if (angle == 0xffff) {
    best = (struct RopeNode *)object->watchedObject;
    value = 0;
  } else {
    value = 16;
    SET_POSITION(&position, 0, 0, 0);
    Vector_AddPolarOffset(px(8), angle, &position);
    position.x += object->x;
    position.y = object->y - position.z;
    position.z = object->z;
    rope = *(struct RopeWork **)&Ram_HeapSlots->unk84[6];
    node = rope->nodes;
    best = NULL;
    bestDistance = 0x10000000;
    if (rope == NULL) {
      collisionFlags = 1;
      /* FAKEMATCH(dead): the event work slot copied onto itself. */
      Ram_HeapSlots->fieldWork = Ram_HeapSlots->fieldWork;
      goto done;
    }
    for (count = 0; count < 128; count++, node++) {
      if (node->kind == 1) {
        s32 dx = (position.x - node->x) >> 8;
        s32 dy = (position.y - node->y) >> 8;
        s32 dz = (position.z - node->z) >> 8;
        s32 distance =
            Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) + Iwram_MulQ16(dz, dz);

        if (distance <= 0x400 && bestDistance > distance) {
          bestDistance = distance;
          best = node;
        }
      }
    }
  }
  if (best == NULL) {
    // DEAD: collisionFlags is still 0 here, so the OR reads nothing; the source
    // keeps it.
    collisionFlags |= 1;
  } else {
    slot = &rope->slots[best->slot - 1];
    first = Actor_GetByIdFar(slot->first);
    second = Actor_GetByIdFar(slot->second);
    if (first->x > second->x)
      speed = -speed;
    first = Actor_GetByIdFar(slot->first);
    second = Actor_GetByIdFar(slot->second);
    if (first->y > second->y) {
      speed = -speed;
      turn = 0xc000;
    } else {
      turn = 0x4000;
    }
    object->scaleX = speed;
    if (best->x == object->x && best->y == object->y && best->z == object->z) {
      collisionFlags = 1;
    } else {
      slot->swinging = 1;
      slot->amplitude += 0x8000;
      if (slot->amplitude > 0x20000)
        slot->amplitude = 0x20000;
      object->watchedObject = (struct Actor *)best;
      SET_POSITION(&position, best->x, best->y, best->z);
      sprite->angle = best->angle + turn;
    }
  }
done:
  work = Ram_HeapSlots->fieldWork;
  if (work != NULL) {
    if (collisionFlags & 3)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = gInput.held;
  }
  Actor_SelectAnimation(object, 29);
  Actor_SetAnimationSpeed(object, value);
  if (collisionFlags != 0) {
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    object->velocityX = 0;
    object->velocityY = 0;
    object->velocityZ = 0;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
  }
  object->step++;
  return 1;
}

#include "hanging_vine.h"

static inline void TurnToward(struct Actor *object, u16 angle) {
  s16 delta = angle - object->facing;

  if (delta > 0x1000)
    delta = 0x1000;
  if (delta < -0x1000)
    delta = -0x1000;
  object->facing += delta;
}

/// Script command 0x1d, Actor_CrossRope on a tightrope (render mode 4).
s32 Actor_CrossTightrope(struct Actor *object) {
  struct FieldPosition position;
  s32 collisionFlags;
  struct ObjectAnimationState *sprite;
  s32 argument;
  struct TightropeWork *tightrope;
  struct TightropeNode *best;
  struct TightropeNode *point;
  struct TightropeSlot *slot;
  s32 bestDistance;
  u16 angle;
  s32 count;
  struct FieldWork *work;

  sprite = object->sprite;
  argument = 1;
  collisionFlags = 0;
  object->speedLimit = px(1) / 2;
  object->acceleration = px(1) / 2;
  object->motionFlags = 0;
  angle = gWorldMap_KeyHeadings[(gInput.held >> 4) & 15];
  if (angle == 0xffff) {
    collisionFlags = 4;
    best = (struct TightropeNode *)object->watchedObject;
    SET_POSITION(&position, object->x, best->y, object->z);
  } else {
    SET_POSITION(&position, object->x, object->y, object->z);
    Vector_AddPolarOffset(px(8), angle, &position);
    tightrope = *(struct TightropeWork **)&Ram_HeapSlots->unk84[7];
    point = tightrope->nodes;
    best = NULL;
    bestDistance = 0x10000000;
    if (tightrope == NULL) {
      collisionFlags = 1;
      /* FAKEMATCH(dead): the event work slot copied onto itself. */
      Ram_HeapSlots->fieldWork = Ram_HeapSlots->fieldWork;
    } else {
      for (count = 0; count < 128; count++, point++) {
        if (point->kind == 1) {
          s32 dx = (position.x - point->x) >> 8;
          s32 dy = (position.y - point->y) >> 8;
          s32 dz = (position.z - point->z) >> 8;
          s32 distance = Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                         Iwram_MulQ16(dz, dz);

          if (distance <= 0x400 && bestDistance > distance) {
            bestDistance = distance;
            best = point;
          }
        }
      }
      if (best == NULL) {
        collisionFlags |= 1;
      } else if (best->x == object->x && best->y == object->y &&
                 best->z == object->z) {
        collisionFlags = 1;
      } else {
        if (best->slot != 0) {
          slot = &tightrope->slots[best->slot - 1];
          slot->wobbling = 1;
          slot->amplitude += 0x8000;
          if (slot->amplitude > 0x20000)
            slot->amplitude = 0x20000;
          object->watchedObject = (struct Actor *)best;
        }
        /* FAKEMATCH(dead): position.x copied onto itself before the copies. */
        position.x = position.x;
        SET_POSITION(&position, best->x, best->y, best->z);
      }
    }
  }
  work = Ram_HeapSlots->fieldWork;
  if (work != NULL) {
    if (collisionFlags & 3)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = gInput.held;
  }
  if (collisionFlags == 0)
    argument = 2;
  Actor_SelectAnimation(object, argument);
  Actor_SetAnimationSpeed(object, 8);
  sprite->unk00[23] = 4;
  if (collisionFlags != 0) {
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    object->velocityX = 0;
    object->velocityY = 0;
    object->velocityZ = 0;
    if (collisionFlags & 3)
      TurnToward(object, angle);
    object->y = position.y;
  } else {
    object->y = position.y;
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    if (object->x == position.x && object->z == position.z)
      TurnToward(object, angle);
  }
  object->step++;
  return 1;
}

#include "flag_call.h"

#define HALF_AT(base, offset) (*(u16 *)((u8 *)(base) + (offset)))

/// Update of a grain the Sand leaves as the leader moves: it turns and
/// jitters sideways at random.
void Sand_JitterGrain(struct Actor *object) {
  u32 first;
  u32 second;
  s32 x;

  HALF_AT(object->sprite, 18) += 0x400;
  first = Random16();
  second = Random16();
  x = object->x;

  second >>= 1;
  first >>= 1;
  object->x = x + (first - second);
}

/// When one of three party key sets is pressed: plays cue 113 if the far call
/// accepts the argument, else posts event request 0x208b and returns 1 while
/// flag 0x146 is clear.
s32 Sand_CheckEndKeys(s32 arg) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  /* FAKEMATCH(pin): pressed held in r0, the register the early zero is returned
   * in. */
  register s32 pressed asm("r0") = 0;

  if (work == NULL)
    return 0;
  if ((gInput.pressed & gMainRoutineWork.selectKeys) ||
      (gInput.pressed & gMainRoutineWork.aKeys) ||
      (gInput.pressed & gMainRoutineWork.startKeys))
    pressed = 1;
  if (pressed) {
    if (Actor_CheckBlockedFar(arg)) {
      Sound_PlayFar(SE_MENU_CANCEL);
    } else if (!GameFlag_Get(0x146)) {
      work->psynergy = 0x208b;
      return 1;
    }
  }
  return 0;
}
#undef gInput.held
#undef CHAIN_WORK
#undef HALF_AT
