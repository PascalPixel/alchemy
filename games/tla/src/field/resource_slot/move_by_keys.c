/// @file
/// Moving actors by the direction pad, and the party's keys.
// Part of resource_slot.c.

#include "../resource_slot_internal.h"

/// The heading of each direction-pad combination, 0xffff for none.
/* VIEW(move_by_keys): variable gData_0802ec5c as extern const u16
 * gData_0802ec5c[16]; f028aac has extern const s16 gData_0802ec5c[16]. */
extern const u16 gData_0802ec5c__move_by_keys[16] __asm__("gData_0802ec5c");

void Vector_AddPolarOffset(s32 radius, s32 angle,
                           struct FieldPosition *position);
s32 Func_080c8710(struct Actor *object);
s32 Func_080c8700(struct Actor *object);
s32 MapGrid_IsWallAt(s32 *position);
s32 Func_0802d45c(s32 terrainId, s32 x, s32 z);
void Actor_SelectAnimation(struct Actor *object, s32 argument);
void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z);

/// The held keys, read again at each use through a volatile pointer, as The
/// Broken Seal's gKeysHeld is declared; as gInput.held, 45 of 241 instructions
/// of Actor_MoveByKeys differ in tla-en.
#define INPUT_HELD (((volatile struct InputState *)&gInput)->held)

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
  struct EventWork *work;
  u16 angle;
  s32 animation;
  s32 collisionFlags;
  s32 deltaZ;
  s32 terrainHeight;
  struct MapWork *map;
  u32 keys;
  s32 half;

  object->acceleration = 0x4000;
  animation = 12;
  half = 0x8000;
  object->speedLimit = half;
  if (gDebugMode)
    keys = INPUT_HELD;
  keys = INPUT_HELD;
  {
    const u16 *table = gData_0802ec5c__move_by_keys;

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
    position.x = 0;
    position.y = 0;
    position.z = 0;
    Vector_AddPolarOffset(0x80000, angle, &position);
    position.x += object->x;
    deltaZ = position.z;
    if (deltaZ < 0)
      object->facing = 0xc000;
    if (deltaZ > 0)
      object->facing = 0x4000;
    position.y = object->y - position.z;
    position.z = object->z;
    map = Ram_HeapSlots->mapWork;
    sourceCell = &map->layers[object->terrainId].cells[Func_080c8710(object)];
    destinationCell =
        &map->layers[object->terrainId]
             .cells[position.x / 0x100000 + (position.z / 0x100000) * 128];
    if (MapGrid_IsWallAt((s32 *)&position) != 0 ||
        !(sourceCell->attributeB & 64) || !(destinationCell->attributeB & 64)) {
      collisionFlags = 4;
      animation = 12;
    } else if (INPUT_HELD & 64) {
      terrainHeight =
          Func_0802d45c(object->terrainId, position.x, position.z - 0x100000);
      if (terrainHeight - object->y < 0x100000)
        goto refused;
    } else if (INPUT_HELD & 128) {
      terrainHeight = Func_0802d45c(object->terrainId, position.x, position.z);
      if (terrainHeight - object->y > -0x80000) {
      refused:
        collisionFlags = 1;
        animation = 12;
      }
    }
  }
  work = Ram_HeapSlots->eventWork;
  if (work != NULL) {
    if (collisionFlags & 3)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = INPUT_HELD;
  }
  Actor_SelectAnimation(object, animation);
  if (collisionFlags != 0) {
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    object->velocityX = 0;
    object->velocityY = 0;
    object->velocityZ = 0;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
  }
  object->step++;
  return 1;
}

struct ChainSlot {
  s16 unk00;
  s16 unk02;
  u8 unk04[8];
  s32 unk0c;
  u8 unk10[8];
  s32 unk18;
  u8 unk1c[4];
};

struct ChainNode {
  u8 unk00[4];
  s32 x;
  s32 y;
  s32 z;
  u16 unk10;
  s8 visible;
  s8 unk13;
  u8 unk14[12];
};

struct ChainWork {
  u8 unk000[12];
  struct ChainSlot slots[8];
  struct ChainNode nodes[128];
};

struct SpriteAngle {
  u8 unk00[18];
  u16 angle;
};

#define CHAIN_WORK (*(struct ChainWork **)&Ram_HeapSlots->unk84[6])

struct Actor *Actor_GetByIdFar(s32 id);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);

s32 Func_080292fc(struct Actor *object) {
  struct FieldPosition position;
  s32 collisionFlags;
  struct SpriteAngle *sprite;
  struct ChainWork *chain;
  s32 bestDistance;
  s32 value;
  s32 speed;
  struct ChainNode *best;
  struct ChainNode *node;
  struct ChainSlot *slot;
  s32 count;
  u16 angle;
  s32 turn;
  struct Actor *first;
  struct Actor *second;
  struct EventWork *work;

  sprite = object->sprite;
  chain = CHAIN_WORK;
  speed = 0x10000;
  collisionFlags = 0;
  object->speedLimit = 0x8000;
  object->acceleration = 0x8000;
  object->motionFlags = 0;
  angle = gData_0802ec5c__move_by_keys[(INPUT_HELD >> 4) & 15];
  if (angle == 0xffff) {
    best = (struct ChainNode *)object->watchedObject;
    value = 0;
  } else {
    value = 16;
    position.x = 0;
    position.y = 0;
    position.z = 0;
    Vector_AddPolarOffset(0x80000, angle, &position);
    position.x += object->x;
    position.y = object->y - position.z;
    position.z = object->z;
    chain = CHAIN_WORK;
    node = chain->nodes;
    best = NULL;
    bestDistance = 0x10000000;
    if (chain == NULL) {
      collisionFlags = 1;
      /* FAKEMATCH(dead): the event work slot copied onto itself, a second
       * store in the null branch that keeps the loop count from being lifted
       * above the null test; without it, 21 of 300 instructions differ in
       * tla-en. */
      Ram_HeapSlots->eventWork = Ram_HeapSlots->eventWork;
      goto done;
    }
    for (count = 0; count < 128; count++, node++) {
      if (node->visible == 1) {
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
    slot = &chain->slots[best->unk13 - 1];
    first = Actor_GetByIdFar(slot->unk00);
    second = Actor_GetByIdFar(slot->unk02);
    if (first->x > second->x)
      speed = -speed;
    first = Actor_GetByIdFar(slot->unk00);
    second = Actor_GetByIdFar(slot->unk02);
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
      slot->unk18 = 1;
      slot->unk0c += 0x8000;
      if (slot->unk0c > 0x20000)
        slot->unk0c = 0x20000;
      object->watchedObject = (struct Actor *)best;
      position.x = best->x;
      position.y = best->y;
      position.z = best->z;
      sprite->angle = best->unk10 + turn;
    }
  }
done:
  work = Ram_HeapSlots->eventWork;
  if (work != NULL) {
    if (collisionFlags & 3)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = INPUT_HELD;
  }
  Actor_SelectAnimation(object, 29);
  Actor_SetAnimationSpeed(object, value);
  if (collisionFlags != 0) {
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    object->velocityX = 0;
    object->velocityY = 0;
    object->velocityZ = 0;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
  }
  object->step++;
  return 1;
}

#include "worka0.h"

static inline void TurnToward(struct Actor *object, u16 angle) {
  s16 delta = angle - object->facing;

  if (delta > 0x1000)
    delta = 0x1000;
  if (delta < -0x1000)
    delta = -0x1000;
  object->facing += delta;
}

s32 Func_08029570(struct Actor *object) {
  struct FieldPosition position;
  s32 collisionFlags;
  struct ObjectAnimationState *sprite;
  s32 argument;
  struct WorkA0 *workA0;
  struct WorkA0Node *best;
  struct WorkA0Node *point;
  struct WorkA0Slot *slot;
  s32 bestDistance;
  u16 angle;
  s32 count;
  struct EventWork *work;

  sprite = object->sprite;
  argument = 1;
  collisionFlags = 0;
  object->speedLimit = 0x8000;
  object->acceleration = 0x8000;
  object->motionFlags = 0;
  angle = gData_0802ec5c__move_by_keys[(gInput.held >> 4) & 15];
  if (angle == 0xffff) {
    collisionFlags = 4;
    best = (struct WorkA0Node *)object->watchedObject;
    position.x = object->x;
    position.y = best->y;
    position.z = object->z;
  } else {
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x80000, angle, &position);
    // Read through the slot's address: as Ram_HeapSlots->unk84[7], 44 of 324
    // instructions differ in tla-en.
    workA0 = *(struct WorkA0 **)&Ram_HeapSlots->unk84[7];
    point = workA0->items;
    best = NULL;
    bestDistance = 0x10000000;
    if (workA0 == NULL) {
      collisionFlags = 1;
      /* FAKEMATCH(dead): the event work slot copied onto itself, a second
       * store in the null branch that keeps the loop count from being lifted
       * above the null test; without it, 29 of 324 instructions differ in
       * tla-en. */
      Ram_HeapSlots->eventWork = Ram_HeapSlots->eventWork;
    } else {
      for (count = 0; count < 128; count++, point++) {
        if (point->used == 1) {
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
        // The slot index, a signed byte (ldrsb) that worka0.h's unk11 does not
        // yet type.
        if ((s8)best->unk11[0] != 0) {
          slot = &workA0->slots[(s8)best->unk11[0] - 1];
          slot->tail = (struct WorkA0Node *)1;
          slot->amplitude += 0x8000;
          if (slot->amplitude > 0x20000)
            slot->amplitude = 0x20000;
          object->watchedObject = (struct Actor *)best;
        }
        /* FAKEMATCH(dead): position.x copied onto itself before the copies, a
         * store that stops GCSE carrying the compare's x load past the pair
         * block; without it, 45 of 324 instructions differ in tla-en. */
        position.x = position.x;
        position.x = best->x;
        position.y = best->y;
        position.z = best->z;
      }
    }
  }
  work = Ram_HeapSlots->eventWork;
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
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
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

u32 Random16(void);

void Func_08029810(struct Actor *object) {
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

s32 Func_080c8828(s32 arg);

/// When one of three party key sets is pressed: plays cue 113 if the far call
/// accepts the argument, else posts event request 0x208b and returns 1 while
/// flag 0x146 is clear.
s32 Func_08029838(s32 arg) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  /* FAKEMATCH(pin): pressed held in r0, the register the early zero is
   * returned in, so the argument moves to r6; as a plain local, which takes r6
   * itself, 14 of 61 instructions differ in tla-en. */
  register s32 pressed asm("r0") = 0;

  if (work == NULL)
    return 0;
  if ((gInput.pressed & gMainRoutineWork.keys234) ||
      (gInput.pressed & gMainRoutineWork.keys230) ||
      (gInput.pressed & gMainRoutineWork.keys236))
    pressed = 1;
  if (pressed) {
    if (Func_080c8828(arg)) {
      Sound_PlayFar(113);
    } else if (!GameFlag_Test(0x146)) {
      work->unk166 = 0x208b;
      return 1;
    }
  }
  return 0;
}
#undef INPUT_HELD
#undef CHAIN_WORK
#undef HALF_AT
