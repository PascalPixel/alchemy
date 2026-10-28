// Draft, differ 14 of 300 in tla-en under -mthumb-slow-loads (2026-10-26). A
// device search (each statement followed by an empty asm or a one-pass block,
// wrapped in one, or swapped with the next, kept when it helps) reaches 10
// with an empty asm after `sprite = object->animation;`, and finds nothing
// closer.
// Near miss, differ 30 of 300, all instruction order but one register (127
// is built in r0, the native r1): script command 0x1c, a sibling of
// Actor_MoveByKeys that moves an object along the nearest rail point within
// 32 pixels of the direction pad's step. The native issues the opening
// loads and constant halves in source order where sched2 here interleaves
// them, and copies positions as in Func_08025698. Measured: the spilled
// locals take their stack slots in reverse declaration order (declared in
// code order, differ 66); the rail loop counts up (counting down, loop.c
// keeps IwramMulQ16 in fp and spills the count, differ 128); the turn is an
// s32 (as u16, differ 141). Permute 18 minutes: a one-pass block after the
// sprite load, differ 26 by weight only.
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
void Actor_SelectAnimation(struct Actor *object,
                                            s32 argument);
void Actor_SetAnimationSpeed(struct Actor *object,
                                         s32 value);
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
    position.x = collisionFlags;
    position.y = collisionFlags;
    position.z = collisionFlags;
    Vector_AddPolarOffset(0x80000, angle, &position);
    position.x += object->x;
    position.y = object->y - position.z;
    position.z = object->z;
    rail = RAIL_WORK;
    point = rail->points;
    best = NULL;
    bestDistance = 0x10000000;
    if (rail == NULL)
      goto stop;
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
    stop:
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
