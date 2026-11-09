#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "actor.h"
#include "iwram_call.h"
#include "event_work.h"

struct FieldPosition {
  s32 x;
  s32 y;
  s32 z;
};

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

extern const u16 gData_0802ec5c[16];

#define INPUT_HELD (((volatile struct InputState *)&gInput)->held)
#define CHAIN_WORK (*(struct ChainWork **)&Ram_HeapSlots->unk84[6])

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

  sprite = object->animation;
  chain = CHAIN_WORK;
  speed = 0x10000;
  collisionFlags = 0;
  object->speedLimit = 0x8000;
  object->acceleration = 0x8000;
  object->motionFlags = 0;
  angle = gData_0802ec5c[(INPUT_HELD >> 4) & 15];
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
