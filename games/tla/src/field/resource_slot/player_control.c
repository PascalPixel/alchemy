/// @file
/// Player control: the configured keys, and SC_PLAYER, the leader moved by the
/// pad (Actor_UpdatePlayerControl).
// Part of resource_slot.c.

#include "../resource_slot_internal.h"
#include "flag_call.h"
#include "frame.h"
#include "module/field/field.h"
#include "module/field/object.h"
#include "module/field/resource_slot.h"

/* VIEW(f0267bc): tag struct PartyKeys as struct PartyKeys__f0267bc;
 * resource_slot.c has another definition; tried with the definition the rest of
 * the object has, it does not compile in tla-ja:
 * games/tla/src/field/resource_slot.c:4350: structure has no member named
 * `keys23c'. */
struct PartyKeys__f0267bc {
  u8 unk000[0x23c];
  u16 keys23c;
};

// The heading of each direction-pad combination, -1 for none.
extern const s16 gWorldMap_KeyHeadings[16];

s32 MapGrid_CheckCollision(struct Actor *object,
                           struct FieldPosition *position);
/* VIEW(f0267bc): routine Unit_Get as struct LeaderUnit *Unit_Get(s32 owner);
 * offset has void *Unit_Get(s32 owner); tried as the definition's void
 * *Unit_Get(s32 owner), which the object already declares, it does not compile
 * in tla-ja: games/tla/src/field/resource_slot.c:4584: request for member `hp'
 * in something not a structure or union. */
extern struct LeaderUnit *Unit_Get__f0267bc(s32 owner) __asm__("Unit_Get");
void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z);

// Two 16.16 coordinates fall in the same 16-pixel cell, rounding to nearest.
#define SAME_CELL(a, b) ((((a) - (b) + 0x80000) >> 20) == 0)

s32 Func_080267bc(struct Actor *object) {
  struct FieldPosition position;
  struct FieldPosition testPosition;
  struct FieldPosition contact;
  s16 angleOffsets[22];
  s32 collision;
  s32 pushed;
  s32 movementAnimation;
  u16 angle;
  s16 heading;
  s32 angleIndex;
  s32 onObject;
  struct Actor *other;
  struct EventWork *work;

  movementAnimation = 2;
  collision = 0;
  pushed = 0;
  if (gInput.held & ((struct PartyKeys__f0267bc *)&gMainRoutineWork)->keys23c) {
    object->speedLimit = 0x10000;
    object->acceleration = 0xccc;
    movementAnimation = 5;
  } else {
    object->speedLimit = 0x10000;
    object->acceleration = 0xccc;
  }

  angle = gWorldMap_KeyHeadings[(gInput.held >> 4) & 15];
  if (angle == 0xffff) {
    collision |= 4;
    goto counted;
  }
  // DEAD: collision is already 0 here.
  collision = 0;
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  Vector_AddPolarOffset__resource_slot(0x80000, angle, &position);
  if (object->priorityFlags & 0x40) {
    s32 withinHalfCell;

    onObject = 0;
    other = (struct Actor *)Ram_HeapSlots->scriptObjects;
    withinHalfCell = 0x7ffff;
    for (angleIndex = 0; angleIndex < 64; angleIndex++, other++) {
      s32 delta;
      s32 dy;

      if (other->script == NULL)
        continue;
      if (!(other->priorityFlags & 0x80))
        continue;
      if (other == object)
        continue;
      delta = object->y - other->y;
      dy = delta - 0x100000;
      if (dy < 0)
        dy = 0x100000 - delta;
      if (dy > withinHalfCell)
        continue;
      if (SAME_CELL(object->x, other->x) && SAME_CELL(object->z, other->z))
        onObject = 1;
      if (SAME_CELL(position.x, other->x) && SAME_CELL(position.z, other->z))
        goto clear;
    }
    if (onObject && !MapGrid_CheckCollision(object, &position)) {
      // DEAD: collision is already 0 here.
      collision = 0;
      goto counted;
    }
  }
  /* FAKEMATCH(dead): a test that sets onObject to 1 when it is nonzero, which
   * nothing reads after; as no test, 12 of 713 instructions differ in
   * tla-en. */
  if (onObject)
    onObject = 1;
  if (gDebugMode != 0 && (gInput.held & 0x200)) {
    /* FAKEMATCH(dead): a store of the 0 collision already holds, which
     * cross-jumping folds into clear's; as goto clear, 36 of 713 instructions
     * differ in tla-en. */
    collision = 0;
    goto counted;
  }
  if (!MapGrid_CheckCollision(object, &position)) {
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset__resource_slot(0x80000, angle + 0x1000,
                                         &testPosition);
    if (!MapGrid_CheckCollision(object, &testPosition)) {
      testPosition.x = object->x;
      testPosition.y = object->y;
      testPosition.z = object->z;
      Vector_AddPolarOffset__resource_slot(0x80000, angle - 0x1000,
                                           &testPosition);
      if (!MapGrid_CheckCollision(object, &testPosition)) {
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset__resource_slot(0x80000, angle + 0x2000,
                                             &testPosition);
        if (!MapGrid_CheckCollision(object, &testPosition)) {
          testPosition.x = object->x;
          testPosition.y = object->y;
          testPosition.z = object->z;
          Vector_AddPolarOffset__resource_slot(0x80000, angle - 0x2000,
                                               &testPosition);
          if (!MapGrid_CheckCollision(object, &testPosition)) {
            heading = angle;
            goto push_objects;
          }
        }
      }
    }
  }
  {
    s32 headingCount = 0;

    angleOffsets[headingCount++] = angle + 0x1000;
    angleOffsets[headingCount++] = angle - 0x1000;
    angleOffsets[headingCount++] = angle + 0x2000;
    angleOffsets[headingCount++] = angle - 0x2000;
    angleOffsets[headingCount++] = angle + 0x3000;
    angleOffsets[headingCount++] = angle - 0x3000;
    /* FAKEMATCH(dead): a test of headingCount that is never true; as no test,
     * 183 of 713 instructions differ in tla-en. */
    if (headingCount == 0)
      goto clear;
    for (angleIndex = 0; angleIndex < headingCount; angleIndex++) {
      heading = angleOffsets[angleIndex];
      position.x = object->x;
      position.y = object->y;
      position.z = object->z;
      Vector_AddPolarOffset__resource_slot(0x80000, (u16)heading, &position);
      if (MapGrid_CheckCollision(object, &position))
        continue;
      testPosition.x = object->x;
      testPosition.y = object->y;
      testPosition.z = object->z;
      Vector_AddPolarOffset__resource_slot(0x80000, (u16)heading + 0x1000,
                                           &testPosition);
      if (MapGrid_CheckCollision(object, &testPosition))
        continue;
      testPosition.x = object->x;
      testPosition.y = object->y;
      testPosition.z = object->z;
      Vector_AddPolarOffset__resource_slot(0x80000, (u16)heading - 0x1000,
                                           &testPosition);
      if (MapGrid_CheckCollision(object, &testPosition))
        continue;
      testPosition.x = object->x;
      testPosition.y = object->y;
      testPosition.z = object->z;
      Vector_AddPolarOffset__resource_slot(0x80000, (u16)heading + 0x2000,
                                           &testPosition);
      if (MapGrid_CheckCollision(object, &testPosition))
        continue;
      testPosition.x = object->x;
      testPosition.y = object->y;
      testPosition.z = object->z;
      Vector_AddPolarOffset__resource_slot(0x80000, (u16)heading - 0x2000,
                                           &testPosition);
      if (!MapGrid_CheckCollision(object, &testPosition))
        goto push_objects;
    }
  }
  goto blocked;

clear:
  // DEAD: collision is already 0 here.
  collision = 0;
  goto counted;

blocked:
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  collision |= 1;

push_objects:
  contact.x = object->x;
  contact.y = object->y;
  contact.z = object->z;
  Vector_AddPolarOffset__resource_slot(0x40000, (u16)heading, &contact);
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
      u16 direction;

      direction = ArcTan2(other->z - object->z, other->x - object->x);
      testPosition.x = other->x;
      testPosition.y = other->y;
      testPosition.z = other->z;
      Vector_AddPolarOffset__resource_slot(0x4000, direction, &testPosition);
      if (!ActorScript_CheckOverlap(other, &testPosition.x)) {
        testPosition.x = other->x;
        testPosition.y = other->y;
        testPosition.z = other->z;
        Vector_AddPolarOffset__resource_slot(0xa0000, direction, &testPosition);
        if (!MapGrid_CheckCollision(other, &testPosition)) {
          testPosition.x = other->x;
          testPosition.y = other->y;
          testPosition.z = other->z;
          Vector_AddPolarOffset__resource_slot(0xa0000, direction + 0x1000,
                                               &testPosition);
          // The same probe is tested twice.
          if (!MapGrid_CheckCollision(other, &testPosition) &&
              !MapGrid_CheckCollision(other, &testPosition)) {
            testPosition.x = other->x;
            testPosition.y = other->y;
            testPosition.z = other->z;
            Vector_AddPolarOffset__resource_slot(0xa0000, direction - 0x1000,
                                                 &testPosition);
            if (!MapGrid_CheckCollision(other, &testPosition)) {
              Vector_AddPolarOffset__resource_slot(
                  0x4000, direction, (struct FieldPosition *)&other->x);
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
  work = Ram_HeapSlots->eventWork;
  if (work != NULL) {
    if (collision & 3)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
  }
  if (pushed != 0) {
    Actor_SelectAnimation(object, 8);
  } else if (collision != 0) {
    s32 action;

    action = 9;
    if (Unit_Get__f0267bc(gMainRoutineWork.currentOwner)->hp == 0)
      action = 22;
    Actor_SelectAnimation(object, action);
  } else {
    Actor_SelectAnimation(object, movementAnimation);
  }

  if (collision != 0) {
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    if (collision & 3) {
      s32 difference;

      difference = (s16)(angle - object->facing);
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      object->facing += difference;
    }
    object->timer = 0;
    object->unk66 = 2;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    if (object->timer != 0)
      object->timer--;
  }
  Field_CheckConfiguredKeys();
  object->step++;
  return 1;
}

s32 Field_RequestShortcut(u32 value) {
  struct EventPrompt *event = Ram_HeapSlots->eventWork;
  u32 kind = value >> 14;
  s32 code = value & 0x3fff;

  if (GameFlag_Test(0x107) != 0) {
    event->result = 250;
  } else if (event->mode == 3) {
    // The held keys, read again for the second test through a volatile
    // pointer; through a plain pointer, 14 of 69 instructions differ in
    // tla-en.
    volatile u32 *held = &gInput.held;

    if (*held & 0x100)
      event->result = 0xfc88;
    else if (*held & 0x200)
      event->result = 0xfc87;
  } else {
    switch (kind) {
    case 0:
      event->choice[0] = code;
      break;
    case 1:
      event->choice[1] = code;
      break;
    }
  }
  return code;
}

/// Checks the keys the player has configured: the menu, examine and start
/// keys set their request and return 1, the two shortcut keys pass on their
/// assignment (Field_RequestShortcut). While a second event is pending, any of
/// the five requests command 0x209b instead. Returns 0 when none is pressed.
// As ☀️'s function of the same name.
s32 Field_CheckConfiguredKeys(void) {
  struct EventPrompt *work = Ram_HeapSlots->eventWork;
  s32 result = 0;

  if (work == NULL)
    return 0;
  if (work->pendingSecond != 0) {
    if ((gInput.pressed & gMainRoutineWork.keys234) ||
        (gInput.pressed & gMainRoutineWork.keys230) ||
        (gInput.pressed & gMainRoutineWork.keys236) ||
        (gInput.pressed & gMainRoutineWork.keys238) ||
        (gInput.pressed & gMainRoutineWork.keys23a)) {
      work->choice[0] = 0x209b;
      return 1;
    }
    return 0;
  } else {
    if (gInput.pressed & gMainRoutineWork.keys234) {
      work->menu = 1;
      result = 1;
    } else if (gInput.pressed & gMainRoutineWork.keys230) {
      work->examine = 1;
      result = 1;
    } else if (gInput.pressed & gMainRoutineWork.keys236) {
      work->start = 1;
      result = 1;
    } else if (gInput.pressed & gMainRoutineWork.keys238) {
      result = Field_RequestShortcut(gMainRoutineWork.packedEvents[0]);
    } else if (gInput.pressed & gMainRoutineWork.keys23a) {
      result = Field_RequestShortcut(gMainRoutineWork.packedEvents[1]);
    }
  }
  return result;
}

s32 Field_CheckSpheresOverlap(s32 *a, s32 radius_a, s32 *b, s32 radius_b) {
  s32 dx = (*a++ - *b++) >> 16;
  s32 dy = (*a++ - *b++) >> 16;
  s32 dz = (*a - *b) >> 16;
  s32 radius = radius_a + radius_b;

  if (!(dx > 0x400000) && !(dz > 0x400000) &&
      dx * dx + dy * dy + dz * dz < radius * radius)
    return 0;
  return -1;
}

void Func_08026fc8(struct Actor *object, s32 mode) {
  struct Actor *created;
  struct Child *sprite;

  created = Actor_Create(14, object->x, object->y + 0x40000, object->z);
  sprite = object->sprite;
  if (!(object->unk66 & 1))
    Sound_PlayFar(623);
  if (created != NULL) {
    struct Child *child = created->sprite;

    created->terrainHeight = object->terrainHeight;
    Actor_StartScript(created, gData_0802ec88);
    created->motionFlags = 0;
    if (child != NULL) {
      Animation_ApplyChildArgument((struct ActorSpriteView *)child, 1);
      child->value1a = 0;
      child->layer = sprite->layer;
      if (mode == 2) {
        Animation_ApplyChildValuesToRecord((struct ActorSpriteView *)child, 13);
        Animation_ApplyChildValue((struct ActorSpriteView *)child, 8);
      }
    }
  }
}
#undef SAME_CELL

#include "version.h"
#include "frame.h"
#include "module/field/field.h"
#include "module/field/object.h"
#include "module/field/resource_slot.h"

/* VIEW(f027064): tag struct PartyKeys as struct PartyKeys__f027064;
 * resource_slot.c has another definition; tried with the definition the rest of
 * the object has, it does not compile in tla-ja:
 * games/tla/src/field/resource_slot.c:4836: structure has no member named
 * `keys23c'. */
struct PartyKeys__f027064 {
  u8 unk000[0x23c];
  u16 keys23c;
};

extern struct MapCell gMapCellBuffer[];
// The heading of each direction-pad combination, -1 for none.
extern const s16 gWorldMap_KeyHeadings[16];

s32 MapGrid_CheckCollision(struct Actor *object,
                           struct FieldPosition *position);
/* VIEW(f027064): routine Unit_Get as struct LeaderUnit *Unit_Get(s32 owner);
 * offset has void *Unit_Get(s32 owner); tried as the definition's void
 * *Unit_Get(s32 owner), which the object already declares, it does not compile
 * in tla-ja: games/tla/src/field/resource_slot.c:5066: request for member `hp'
 * in something not a structure or union. */
extern struct LeaderUnit *Unit_Get__f027064(s32 owner) __asm__("Unit_Get");
void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z);
/* VIEW(f027064): routine Animation_ApplyChildArgument as s32
 * Animation_ApplyChildArgument(struct PlayerSprite *, s32); the definition has
 * s32 Animation_ApplyChildArgument(struct AnimationGroup *state, s32 flags). */
extern s32 Animation_ApplyChildArgument__f027064(
    struct PlayerSprite *, s32) __asm__("Animation_ApplyChildArgument");

// Two 16.16 coordinates fall in the same 16-pixel cell, rounding to nearest.
#define SAME_CELL(a, b) ((((a) - (b) + 0x80000) >> 20) == 0)

s32 Actor_UpdatePlayerControl(struct Actor *object) {
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
  u32 attribute;

  sprite = object->sprite;
  movementAnimation = 2;
  onObject = 0;
  work = Ram_HeapSlots->eventWork;
  map = (struct MapFlagsWork *)Ram_HeapSlots->mapWork;
  collision = 0;
  pushed = 0;
  if (gInput.held & ((struct PartyKeys__f027064 *)&gMainRoutineWork)->keys23c) {
    object->speedLimit = 0x18000;
    object->acceleration = 0x4000;
    movementAnimation = 5;
  } else {
    object->speedLimit = 0x10000;
    object->acceleration = 0x4000;
  }

  angle = gWorldMap_KeyHeadings[(gInput.held >> 4) & 15];
  if ((u16)angle == 0xffff) {
    collision |= 4;
    goto counted;
  }
  collision = 0;
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  Vector_AddPolarOffset__resource_slot(0x80000, (u16)angle, &position);
  if (object->priorityFlags & 0x40) {
    // Standing on another object a cell below: the step is clear when the
    // object under the step (and, after Japanese, both its sides) is a still
    // one.
#if TLA_PATCH_1
    testPosition.x = position.x;
    testPosition.y = position.y;
    testPosition.z = position.z;
    Vector_AddPolarOffset__resource_slot(0x20000, (u16)angle + 0x4000,
                                         &testPosition);
    contact.x = position.x;
    contact.y = position.y;
    contact.z = position.z;
    Vector_AddPolarOffset__resource_slot(0x20000, (u16)angle - 0x4000,
                                         &contact);
#endif
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
#if TLA_PATCH_1
          SAME_CELL(testPosition.x, other->x) &&
          SAME_CELL(testPosition.z, other->z) &&
          SAME_CELL(contact.x, other->x) && SAME_CELL(contact.z, other->z) &&
#endif
          other->targetX == 0x80000000 && other->targetZ == other->targetX)
        goto clear;
    }
    if (onObject && !MapGrid_CheckCollision(object, &position)) {
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
  if (!MapGrid_CheckCollision(object, &position)) {
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset__resource_slot(0x80000, (u16)angle + 0x1000,
                                         &testPosition);
    if (!MapGrid_CheckCollision(object, &testPosition)) {
      testPosition.x = object->x;
      testPosition.y = object->y;
      testPosition.z = object->z;
      Vector_AddPolarOffset__resource_slot(0x80000, (u16)angle - 0x1000,
                                           &testPosition);
      if (!MapGrid_CheckCollision(object, &testPosition)) {
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset__resource_slot(0x80000, (u16)angle + 0x2000,
                                             &testPosition);
        if (!MapGrid_CheckCollision(object, &testPosition)) {
          testPosition.x = object->x;
          testPosition.y = object->y;
          testPosition.z = object->z;
          Vector_AddPolarOffset__resource_slot(0x80000, (u16)angle - 0x2000,
                                               &testPosition);
          if (!MapGrid_CheckCollision(object, &testPosition)) {
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
    Vector_AddPolarOffset__resource_slot(0x80000, (u16)heading, &position);
    if (MapGrid_CheckCollision(object, &position))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset__resource_slot(0x80000, (u16)heading + 0x1000,
                                         &testPosition);
    if (MapGrid_CheckCollision(object, &testPosition))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset__resource_slot(0x80000, (u16)heading - 0x1000,
                                         &testPosition);
    if (MapGrid_CheckCollision(object, &testPosition))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset__resource_slot(0x80000, (u16)heading + 0x2000,
                                         &testPosition);
    if (MapGrid_CheckCollision(object, &testPosition))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset__resource_slot(0x80000, (u16)heading - 0x2000,
                                         &testPosition);
    if (!MapGrid_CheckCollision(object, &testPosition))
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
  Vector_AddPolarOffset__resource_slot(0x40000, (u16)heading, &contact);
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
      Vector_AddPolarOffset__resource_slot(0x4000, direction, &testPosition);
      if (!ActorScript_CheckOverlap(other, &testPosition.x)) {
        testPosition.x = other->x;
        testPosition.y = other->y;
        testPosition.z = other->z;
        Vector_AddPolarOffset__resource_slot(0xa0000, direction, &testPosition);
        if (!MapGrid_CheckCollision(other, &testPosition)) {
          testPosition.x = other->x;
          testPosition.y = other->y;
          testPosition.z = other->z;
          Vector_AddPolarOffset__resource_slot(0xa0000, direction + 0x1000,
                                               &testPosition);
          // The same probe is tested twice.
          if (!MapGrid_CheckCollision(other, &testPosition) &&
              !MapGrid_CheckCollision(other, &testPosition)) {
            testPosition.x = other->x;
            testPosition.y = other->y;
            testPosition.z = other->z;
            Vector_AddPolarOffset__resource_slot(0xa0000, direction - 0x1000,
                                                 &testPosition);
            if (!MapGrid_CheckCollision(other, &testPosition)) {
              Vector_AddPolarOffset__resource_slot(
                  0x4000, direction, (struct FieldPosition *)&other->x);
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
    if (Unit_Get__f027064(gMainRoutineWork.currentOwner)->hp == 0)
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

      difference = (s16)((u16)angle - object->facing);
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      object->facing += difference;
    }
    object->timer = 0;
    object->unk66 = 2;
  } else {
    s32 speed;

    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    speed = FixedSqrt(Iwram_MulQ16(object->velocityX, object->velocityX) +
                      Iwram_MulQ16(object->velocityZ, object->velocityZ));
    object->velocityX = collision;
    object->velocityZ = collision;
    Vector_AddPolarOffset__resource_slot(
        speed, (u16)heading, (struct FieldPosition *)&object->velocityX);
    if (object->timer != 0)
      object->timer--;
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
    /* FAKEMATCH(temporary): the cell attribute read into a local before the
     * flags; as a read at the test, 4 of 1094 instructions differ in tla-en. */
    attribute = cells->attributeB;
    oldFlags = sprite->flags;
    if (attribute & 16) {
      if (!(oldFlags & 8)) {
        sprite->shape11 = (sprite->shape11 & 3) | 16;
        sprite->flags = ((u8)oldFlags | 4) & ~1;
      }
      if (object->timer == 0 && collision == 0 && !onObject) {
        Func_08026fc8(object, (u32)(map->groundEffects << 28) >> 30);
        if (movementAnimation == 5)
          object->timer = 8;
        else
          object->timer = 12;
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
  if ((map->groundEffects & 3) && object->timer == 0 && collision == 0) {
    struct Actor *effect;

    effect = Actor_Create(0x10e, object->x, object->y, object->z);
    if (effect != NULL) {
      struct PlayerSprite *effectSprite;

      effect->terrainHeight = object->terrainHeight;
      effectSprite = effect->sprite;
      Actor_StartScript(effect, gData_0802ec7c);
      effect->priorityFlags = 2;
      effect->motionFlags = collision;
      if (effectSprite != NULL) {
        Animation_ApplyChildArgument__f027064(effectSprite, 1);
        effectSprite->flags = collision;
        effectSprite->rotation = footprintAngle + 0x4000;
        effectSprite->priority = 3;
      }
      if (*(s16 *)&object->unk66 == 2) {
        Animation_ApplyChildArgument__f027064(effectSprite, 2);
        object->unk66 = collision;
      }
      if (*(s16 *)&object->unk66 != 0)
        effect->facing = 0x8000;
      if (movementAnimation == 5)
        object->timer = 12;
      else
        object->timer = 18;
      object->unk66 ^= 1;
    }
  }
  Field_CheckConfiguredKeys();
  object->step++;
  return 1;
}
#undef SAME_CELL
