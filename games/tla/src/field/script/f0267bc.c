#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "event_work.h"
#include "main_routine_work.h"
#include "actor.h"

struct FieldPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct PartyKeys {
  u8 unk000[0x23c];
  u16 keys23c;
};

struct LeaderUnit {
  u8 unk00[0x38];
  s16 hp;
};

extern u8 gDebugMode;
// The heading of each direction-pad combination, -1 for none.
extern const s16 gData_0802ec5c[16];

void Vector_AddPolarOffset(s32 radius, s32 angle,
                           struct FieldPosition *position);
s32 Func_0802d87c(struct Actor *object, struct FieldPosition *position);
s32 Field_CheckSpheresOverlap(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
s32 ScriptObject_CheckOverlap(struct Actor *object, s32 *values);
s16 ArcTan2(s32 y, s32 x);
struct LeaderUnit *Unit_Get(s32 owner);
void Actor_SelectAnimation(struct Actor *object, s32 argument);
void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z);
s32 Field_CheckConfiguredKeys(void);

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
  if (gInput.held & ((struct PartyKeys *)&gMainRoutineWork)->keys23c) {
    object->speedLimit = 0x10000;
    object->acceleration = 0xccc;
    movementAnimation = 5;
  } else {
    object->speedLimit = 0x10000;
    object->acceleration = 0xccc;
  }

  angle = gData_0802ec5c[(gInput.held >> 4) & 15];
  if (angle == 0xffff) {
    collision |= 4;
    goto counted;
  }
  // DEAD: collision is already 0 here.
  collision = 0;
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  Vector_AddPolarOffset(0x80000, angle, &position);
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
    if (onObject && !Func_0802d87c(object, &position)) {
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
  if (!Func_0802d87c(object, &position)) {
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, angle + 0x1000, &testPosition);
    if (!Func_0802d87c(object, &testPosition)) {
      testPosition.x = object->x;
      testPosition.y = object->y;
      testPosition.z = object->z;
      Vector_AddPolarOffset(0x80000, angle - 0x1000, &testPosition);
      if (!Func_0802d87c(object, &testPosition)) {
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, angle + 0x2000, &testPosition);
        if (!Func_0802d87c(object, &testPosition)) {
          testPosition.x = object->x;
          testPosition.y = object->y;
          testPosition.z = object->z;
          Vector_AddPolarOffset(0x80000, angle - 0x2000, &testPosition);
          if (!Func_0802d87c(object, &testPosition)) {
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
      u16 direction;

      direction = ArcTan2(other->z - object->z, other->x - object->x);
      testPosition.x = other->x;
      testPosition.y = other->y;
      testPosition.z = other->z;
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

/// The requests the configured keys raise.
// Named as in ☀️'s FieldWork.
struct EventPrompt {
  u8 unk000[0x15a];
  u16 menu;
  u16 examine;
  u16 start;
  u8 unk160[6];
  u16 choice[2];
  u16 result;
  u8 unk16c[30];
  s8 mode;
  u8 unk18b[0xb21];
  s8 pendingSecond;
};

s32 GameFlag_Test(s32 flag);

s32 Func_08026dd0(u32 value) {
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
/// assignment (Func_08026dd0). While a second event is pending, any of the
/// five requests command 0x209b instead. Returns 0 when none is pressed.
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
      result = Func_08026dd0(gMainRoutineWork.packedEvents[0]);
    } else if (gInput.pressed & gMainRoutineWork.keys23a) {
      result = Func_08026dd0(gMainRoutineWork.packedEvents[1]);
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

#include "actor_view.h"

struct Actor *Func_08023220(s32 kind, s32 x, s32 y, s32 z);
extern const s32 gData_0802ec88[];

struct Child {
  u8 unk00[9];
  u8 low9 : 2;
  u8 layer : 2;
  u8 high9 : 4;
  u8 unk0a[0x10];
  u8 value1a;
};

void Func_08026fc8(struct Actor *object, s32 mode) {
  struct Actor *created;
  struct Child *sprite;

  created = Func_08023220(14, object->x, object->y + 0x40000, object->z);
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
