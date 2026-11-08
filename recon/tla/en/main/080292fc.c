// Matches: differ 0 (tla-en, 2026-11-08)
// Also differ 0 in tla-ja, tla-de, tla-es, tla-fr and tla-it. One device:
// the null rail test reads rail through __builtin_expect (FAKEMATCH(wrapper)
// at the test). Script command 0x1c, a sibling of Actor_MoveByKeys that
// moves an object along the nearest rail point within 32 pixels of the
// direction pad's step; it copies positions as in Func_08025698.
// Why the device (read from the RTL dumps, 2026-11-08): a missing rail work
// has its own collisionFlags = 1 block, between the test and the loop, as
// the arrival case has. Reload gives that block's 1 the spill register r0
// (round robin after r1 for 268, r2 for best's NULL, r3 for bestDistance),
// so the loop's starting count of 127 takes r1 as in the native; jump2 then
// cross-jumps the block into the arrival case's identical one and the test
// becomes the native's single beq. As plain C, if-conversion (14.ce,
// IF-CASE-2) hoists the loop's preheader, the reversed count's 127, above
// the test, because the jump around the null block is predicted taken: 21
// differ. The expectation predicts it not taken, which is all that stops the
// hoist. Other ways to stop it: an empty asm in the preheader (its 4-byte
// length estimate pushes the beq out of range, 15) or in both blocks (asm is
// not cross-jumped, so the null block keeps a jump, 15); value = 16 again in
// the null block or the preheader (CSE drops it, 21);
// __builtin_expect(rail == NULL, 1) (builds a store flag, 14). Plain C has no
// other lever: the preheader holds no memory, call or hard register, and the
// jump to it is NE, which both heuristics predict taken. The old shape, one
// shared block behind goto stop, was differ 4 with no device (127 in r0).
// Measured earlier: the spilled locals take their stack slots in reverse
// declaration order (in code order, 66); the rail loop counts up in C
// (counting down, 110); the turn is an s32 (as u16, 141); count as u32 or
// the loop to != 128 (105). The position's zeros are written as 0: CSE
// substitutes collisionFlags' register for them, as the native does.
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "actor.h"
#include "iwram_call.h"

struct FieldPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct KeyMoveWork {
  u8 unk000[0x184];
  s16 blockedSteps;
  u16 heldKeys;
};

struct RailPair {
  s16 first;
  s16 second;
  u8 unk04[8];
  s32 speed;
  u8 unk10[8];
  s32 moving;
  u8 unk1c[4];
};

struct RailPoint {
  u8 unk00[4];
  s32 x;
  s32 y;
  s32 z;
  u16 angle;
  s8 active;
  s8 pair;
  u8 unk14[12];
};

struct RailWork {
  u8 unk000[12];
  struct RailPair pairs[8];
  struct RailPoint points[128];
};

struct SpriteAngle {
  u8 unk00[18];
  u16 angle;
};

extern const u16 gData_0802ec5c[16];

#define INPUT_HELD (((volatile struct InputState *)&gInput)->held)
#define RAIL_WORK (*(struct RailWork **)&Ram_HeapSlots->unk84[6])

void Vector_AddPolarOffset(s32 radius, s32 angle,
                           struct FieldPosition *position);
struct Actor *Actor_GetByIdFar(s32 id);
void Actor_SelectAnimation(struct Actor *object, s32 argument);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
s32 Actor_SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z);

s32 Func_080292fc(struct Actor *object) {
  struct FieldPosition position;
  s32 collisionFlags;
  struct SpriteAngle *sprite;
  struct RailWork *rail;
  s32 bestDistance;
  s32 value;
  s32 speed;
  struct RailPoint *best;
  struct RailPoint *point;
  struct RailPair *pair;
  s32 count;
  u16 angle;
  s32 turn;
  struct Actor *first;
  struct Actor *second;
  struct KeyMoveWork *work;

  sprite = object->animation;
  rail = RAIL_WORK;
  speed = 0x10000;
  collisionFlags = 0;
  object->speedLimit = 0x8000;
  object->acceleration = 0x8000;
  object->motionFlags = collisionFlags;
  angle = gData_0802ec5c[(INPUT_HELD >> 4) & 15];
  if (angle == 0xffff) {
    best = (struct RailPoint *)object->watchedObject;
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
    rail = RAIL_WORK;
    point = rail->points;
    best = NULL;
    bestDistance = 0x10000000;
    /* FAKEMATCH(wrapper): the null test reads rail through __builtin_expect;
     * as rail == NULL, 21 of 300 instructions differ in tla-en. */
    if (!__builtin_expect((s32)rail, 0)) {
      collisionFlags = 1;
      goto done;
    }
    for (count = 0; count < 128; count++, point++) {
      if (point->active == 1) {
        s32 dx = (position.x - point->x) >> 8;
        s32 dy = (position.y - point->y) >> 8;
        s32 dz = (position.z - point->z) >> 8;
        s32 distance =
            Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) + Iwram_MulQ16(dz, dz);

        if (distance <= 0x400 && bestDistance > distance) {
          bestDistance = distance;
          best = point;
        }
      }
    }
  }
  if (best == NULL) {
    collisionFlags |= 1;
  } else {
    pair = &rail->pairs[best->pair - 1];
    first = Actor_GetByIdFar(pair->first);
    second = Actor_GetByIdFar(pair->second);
    if (first->x > second->x)
      speed = -speed;
    first = Actor_GetByIdFar(pair->first);
    second = Actor_GetByIdFar(pair->second);
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
      pair->moving = 1;
      pair->speed += 0x8000;
      if (pair->speed > 0x20000)
        pair->speed = 0x20000;
      object->watchedObject = (struct Actor *)best;
      position.x = best->x;
      position.y = best->y;
      position.z = best->z;
      sprite->angle = best->angle + turn;
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
