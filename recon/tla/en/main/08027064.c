// Draft, differ 12 of 1094, skeleton gap 4 (tla-en, 2026-11-01)
// Script command 0x19 (SC_PLAYER), the twin of The Broken Seal's
// Actor_UpdatePlayerControl (games/tbs/src/field/field/player_control.c), with
// this game's changes: no fast walk; standing on another object a cell below
// (priorityFlags bit 6) lets the step through when a still object (bit 7) fills
// the step and both its sides; wading ground (map byte 0x17 bits 2-3, cell
// attribute bit 4) hides the sprite's lower half and leaves Func_08026fc8's
// effect. Taken from the twin: headingCount, whose folded 6 gives cmp #6 and
// blt; footprintAngle set before the footprint test; the signed reads of unk66;
// flags before rotation. The angle stays a u16 (as s32, differ 251) and the
// locals keep frame order.
// oldFlags is an s32 so that gcse carries the flags byte across the wading
// branches (the listing's copy into r1); the stores narrow it with (u8) to get
// the listing's #254 and #251 masks. One pointer, cells, steps to the cell (as
// a separate cell, differ 26).
// Left, four pairs: three scheduling swaps (collision's store against lsls r0
// at the top; angleIndex's store against mov r7, sl before the heading loop and
// against adds r6, r7 before the push loop) and ands r3, r2 for ands r2, r3 at
// the wading test.
// Tried: all 720 orders of the opening assignments, every declaration order and
// inner scope, loop inits outside the for, and moving the read of oldFlags.
// Reading attributeB into a local before oldFlags gives ands r2, r3 and differ
// 8, but it is a forced temporary, so it stays out.
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "map.h"
#include "event_work.h"
#include "main_routine_work.h"
#include "actor.h"
#include "iwram_call.h"

struct FieldPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct PlayerSprite {
  u8 unk00[9];
  u8 low09 : 2;
  u8 priority : 2;
  u8 high09 : 4;
  u8 unk0a[7];
  u8 shape11;
  u16 rotation;
  u8 unk14[5];
  u8 dirty;
  u8 flags;
};

struct PartyKeys {
  u8 unk000[0x23c];
  u16 keys23c;
};

struct MapFlagsWork {
  u8 unk000[0x17];
  u8 groundEffects; ///< bits: 3..2 the wading kind | 1..0 footprints
};

struct LeaderUnit {
  u8 unk00[0x38];
  s16 hp;
};

extern u8 gDebugMode;
extern struct MapCell gMapCellBuffer[];
// The heading of each direction-pad combination, -1 for none.
extern const s16 gData_0802ec5c[16];
extern const s32 gData_0802ec7c[];

void Vector_AddPolarOffset(s32 radius, s32 angle,
                           struct FieldPosition *position);
s32 Func_0802d87c(struct Actor *object, struct FieldPosition *position);
s32 Field_CheckSpheresOverlap(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
s32 ScriptObject_CheckOverlap(struct Actor *object, s32 *values);
s16 ArcTan2(s32 y, s32 x);
struct LeaderUnit *Unit_Get(s32 owner);
void Actor_SelectAnimation(struct Actor *object, s32 argument);
s32 Actor_SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z);
s32 FixedSqrt(s32 value);
struct Actor *Func_08023220(s32 kind, s32 x, s32 y, s32 z);
void Actor_StartScript(struct Actor *, const s32 *);
s32 Animation_ApplyChildArgument(struct PlayerSprite *, s32);
void Func_08026fc8(struct Actor *object, s32 kind);
s32 Field_CheckConfiguredKeys(void);

// Two 16.16 coordinates fall in the same 16-pixel cell, rounding to nearest.
#define SAME_CELL(a, b) ((((a) - (b) + 0x80000) >> 20) == 0)

s32 Func_08027064(struct Actor *object) {
  struct FieldPosition position;
  struct FieldPosition testPosition;
  struct FieldPosition contact;
  s16 angleOffsets[22];
  s32 collision;
  s32 pushed;
  struct PlayerSprite *sprite;
  s16 heading;
  s32 angleIndex;
  s32 movementAnimation;
  s32 headingCount;
  s32 footprintAngle;
  s32 onObject;
  struct EventWork *work;
  struct MapFlagsWork *map;
  u16 angle;
  u16 direction;
  struct Actor *other;
  s32 oldFlags;

  sprite = object->animation;
  movementAnimation = 2;
  onObject = 0;
  work = Ram_HeapSlots->eventWork;
  map = (struct MapFlagsWork *)Ram_HeapSlots->mapWork;
  collision = 0;
  pushed = 0;
  if (gInput.held & ((struct PartyKeys *)&gMainRoutineWork)->keys23c) {
    object->speedLimit = 0x18000;
    object->acceleration = 0x4000;
    movementAnimation = 5;
  } else {
    object->speedLimit = 0x10000;
    object->acceleration = 0x4000;
  }

  angle = gData_0802ec5c[(gInput.held >> 4) & 15];
  if ((u16)angle == 0xffff) {
    collision |= 4;
    goto counted;
  }
  collision = 0;
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  Vector_AddPolarOffset(0x80000, (u16)angle, &position);
  if (object->priorityFlags & 0x40) {
    // Standing on another object a cell below: the step is clear when the
    // object under the step and both its sides is a still one.
    testPosition.x = position.x;
    testPosition.y = position.y;
    testPosition.z = position.z;
    Vector_AddPolarOffset(0x20000, (u16)angle + 0x4000, &testPosition);
    contact.x = position.x;
    contact.y = position.y;
    contact.z = position.z;
    Vector_AddPolarOffset(0x20000, (u16)angle - 0x4000, &contact);
    angleIndex = 0;
    other = (struct Actor *)Ram_HeapSlots->scriptObjects;
    for (; angleIndex < 64; angleIndex++, other++) {
      s32 dy;

      if (other->script == NULL)
        continue;
      if (!(other->priorityFlags & 0x80))
        continue;
      if (other == object)
        continue;
      dy = object->y - other->y;
      if ((dy - 0x100000 < 0 ? -(dy - 0x100000) : dy - 0x100000) > 0x7ffff)
        continue;
      if (SAME_CELL(object->x, other->x) && SAME_CELL(object->z, other->z))
        onObject = 1;
      if (SAME_CELL(position.x, other->x) && SAME_CELL(position.z, other->z) &&
          SAME_CELL(testPosition.x, other->x) &&
          SAME_CELL(testPosition.z, other->z) &&
          SAME_CELL(contact.x, other->x) && SAME_CELL(contact.z, other->z) &&
          other->targetX == 0x80000000 && other->targetZ == other->targetX)
        goto clear;
    }
    if (onObject && !Func_0802d87c(object, &position)) {
      collision = 0;
      heading = angle;
      goto push_objects;
    }
  }
  if (gDebugMode != 0 && (gInput.held & 0x200)) {
    collision = 0;
    heading = angle;
    goto counted;
  }
  if (!Func_0802d87c(object, &position)) {
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)angle + 0x1000, &testPosition);
    if (!Func_0802d87c(object, &testPosition)) {
      testPosition.x = object->x;
      testPosition.y = object->y;
      testPosition.z = object->z;
      Vector_AddPolarOffset(0x80000, (u16)angle - 0x1000, &testPosition);
      if (!Func_0802d87c(object, &testPosition)) {
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)angle + 0x2000, &testPosition);
        if (!Func_0802d87c(object, &testPosition)) {
          testPosition.x = object->x;
          testPosition.y = object->y;
          testPosition.z = object->z;
          Vector_AddPolarOffset(0x80000, (u16)angle - 0x2000, &testPosition);
          if (!Func_0802d87c(object, &testPosition)) {
            heading = angle;
            goto push_objects;
          }
        }
      }
    }
  }
  direction = angle;
  headingCount = 0;
  angleOffsets[headingCount++] = direction + 0x1000;
  angleOffsets[headingCount++] = direction - 0x1000;
  angleOffsets[headingCount++] = direction + 0x2000;
  angleOffsets[headingCount++] = direction - 0x2000;
  angleOffsets[headingCount++] = direction + 0x3000;
  angleOffsets[headingCount++] = direction - 0x3000;
  for (angleIndex = 0; angleIndex < headingCount; angleIndex++) {
    heading = angleOffsets[angleIndex];
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)heading, &position);
    if (Func_0802d87c(object, &position))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)heading + 0x1000, &testPosition);
    if (Func_0802d87c(object, &testPosition))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)heading - 0x1000, &testPosition);
    if (Func_0802d87c(object, &testPosition))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)heading + 0x2000, &testPosition);
    if (Func_0802d87c(object, &testPosition))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)heading - 0x2000, &testPosition);
    if (!Func_0802d87c(object, &testPosition))
      goto push_objects;
  }
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  collision |= 1;
  goto push_objects;

clear:
  collision = 0;
  heading = angle;

push_objects:
  // Push the movable objects just ahead a quarter pixel further, when they
  // have room; any other solid object there blocks the step.
  contact.x = object->x;
  contact.y = object->y;
  contact.z = object->z;
  Vector_AddPolarOffset(0x40000, (u16)heading, &contact);
  other = (struct Actor *)Ram_HeapSlots->scriptObjects;
  for (angleIndex = 63; angleIndex >= 0; angleIndex--, other++) {
    s32 radius;

    radius = object->radius - 2;
    if (other->script == NULL)
      continue;
    if (!(other->collisionFlags & 1))
      continue;
    if (other == object)
      continue;
    if (Field_CheckSpheresOverlap(&other->x, radius, &contact.x,
                                   other->radius - 2) < 0)
      continue;
    if ((other->collisionFlags & 2) && other->movementState == 0) {
      direction = ArcTan2(other->z - object->z, other->x - object->x);
      testPosition.x = other->x;
      testPosition.y = other->y;
      testPosition.z = other->z;
      heading = direction;
      Vector_AddPolarOffset(0x4000, direction, &testPosition);
      if (!ScriptObject_CheckOverlap(other, &testPosition.x)) {
        testPosition.x = other->x;
        testPosition.y = other->y;
        testPosition.z = other->z;
        Vector_AddPolarOffset(0xa0000, direction, &testPosition);
        if (!Func_0802d87c(other, &testPosition)) {
          testPosition.x = other->x;
          testPosition.y = other->y;
          testPosition.z = other->z;
          Vector_AddPolarOffset(0xa0000, direction + 0x1000, &testPosition);
          // The same probe is tested twice.
          if (!Func_0802d87c(other, &testPosition) &&
              !Func_0802d87c(other, &testPosition)) {
            testPosition.x = other->x;
            testPosition.y = other->y;
            testPosition.z = other->z;
            Vector_AddPolarOffset(0xa0000, direction - 0x1000, &testPosition);
            if (!Func_0802d87c(other, &testPosition)) {
              Vector_AddPolarOffset(0x4000, direction,
                                    (struct FieldPosition *)&other->x);
              other->targetX = 0x80000000;
              other->targetY = 0x80000000;
              other->targetZ = 0x80000000;
              pushed |= 1;
              continue;
            }
          }
        }
      }
    }
    collision |= 2;
  }
  if (collision == 0 && pushed != 0) {
    object->speedLimit = 0x4000;
    object->acceleration = 0x2000;
  }

counted:
  if (work != NULL) {
    if (collision & 3)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = gInput.held;
  }
  if (pushed != 0) {
    Actor_SelectAnimation(object, 8);
  } else if (collision != 0) {
    s32 action;

    action = 9;
    if (Unit_Get(gMainRoutineWork.currentOwner)->hp == 0)
      action = 22;
    Actor_SelectAnimation(object, action);
  } else {
    Actor_SelectAnimation(object, movementAnimation);
  }

  if (collision != 0) {
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    object->velocityX = 0;
    object->velocityZ = 0;
    if (collision & 3) {
      s32 difference;

      difference = (s16)((u16)angle - object->angle);
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      object->angle += difference;
    }
    object->action = 0;
    object->unk66 = 2;
  } else {
    s32 speed;

    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    speed = FixedSqrt(Iwram_MulQ16(object->velocityX, object->velocityX) +
                      Iwram_MulQ16(object->velocityZ, object->velocityZ));
    object->velocityX = collision;
    object->velocityZ = collision;
    Vector_AddPolarOffset(speed, (u16)heading,
                          (struct FieldPosition *)&object->velocityX);
    if (object->action != 0)
      object->action--;
  }

  // Ground that wades: hide the lower half of the sprite while on it, and
  // leave the wading effect as the leader moves.
  if (map->groundEffects & 12) {
    struct MapCell *cells;

    if (object->terrainId <= 2)
      cells = ((struct MapWork *)map)->layers[object->terrainId].cells;
    else
      cells = gMapCellBuffer;
    cells += object->x / 0x100000 + (object->z / 0x100000) * 128;
    oldFlags = sprite->flags;
    if (cells->attributeB & 16) {
      if (!(oldFlags & 8)) {
        sprite->shape11 = (sprite->shape11 & 3) | 16;
        sprite->flags = ((u8)oldFlags | 4) & ~1;
      }
      if (object->action == 0 && collision == 0 && !onObject) {
        Func_08026fc8(object, (u32)(map->groundEffects << 28) >> 30);
        if (movementAnimation == 5)
          object->action = 8;
        else
          object->action = 12;
        object->unk66 ^= 1;
      }
    } else if (!(oldFlags & 8)) {
      sprite->flags = ((u8)oldFlags | 1) & ~4;
    }
    if (oldFlags != sprite->flags)
      sprite->dirty = 1;
  }

  // Footprints on maps that show them, alternating left and right.
  footprintAngle = (u16)angle;
  if ((map->groundEffects & 3) && object->action == 0 && collision == 0) {
    struct Actor *effect;

    effect = Func_08023220(0x10e, object->x, object->y, object->z);
    if (effect != NULL) {
      struct PlayerSprite *effectSprite;

      effect->terrainHeight = object->terrainHeight;
      effectSprite = effect->animation;
      Actor_StartScript(effect, gData_0802ec7c);
      effect->priorityFlags = 2;
      effect->motionFlags = collision;
      if (effectSprite != NULL) {
        Animation_ApplyChildArgument(effectSprite, 1);
        effectSprite->flags = collision;
        effectSprite->rotation = footprintAngle + 0x4000;
        effectSprite->priority = 3;
      }
      if (*(s16 *)&object->unk66 == 2) {
        Animation_ApplyChildArgument(effectSprite, 2);
        object->unk66 = collision;
      }
      if (*(s16 *)&object->unk66 != 0)
        effect->angle = 0x8000;
      if (movementAnimation == 5)
        object->action = 12;
      else
        object->action = 18;
      object->unk66 ^= 1;
    }
  }
  Field_CheckConfiguredKeys();
  object->step++;
  return 1;
}
