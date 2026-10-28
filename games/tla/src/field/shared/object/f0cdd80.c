#include "types.h"
#include "adventure.h"
#include "actor.h"

#define S16_AT(base, offset) (*(s16 *)((u8 *)(base) + (offset)))

struct Actor *Actor_GetById(s32 id);
void Vector_AddPolarOffset(s32 length, s32 angle, s32 *position);
struct Actor *Func_080eaf28(s32 *position, s32 mode);
s32 Func_08020210(struct Actor *object, s32 *position);
void wait(s32 frames);
void Event_NoOp(void);
void Actor_SetPositionAndResetMotionFar(struct Actor *object, s32 x,
                                         s32 y, s32 z);

void Func_080cdd80(void) {
  s32 position[3];
  struct Actor *owner = Actor_GetById(gAdventure.currentOwner);
  struct Actor *target;
  s32 terrain;
  s32 result;
  s32 speed;
  s32 acceleration;
  s32 angle = owner->angle >> 12;

  angle = ((angle + 2) & 12) << 12;
  position[0] = owner->x;
  position[1] = (owner->y + 0x40000) & 0xfff80000;
  position[2] = owner->z;
  Vector_AddPolarOffset(0x100000, angle, position);
  target = Func_080eaf28(position, 1);
  position[0] = target->x;
  position[1] = target->y;
  position[2] = target->z;
  Vector_AddPolarOffset(0x100000, angle, position);
  terrain = target->terrainId;
  target->terrainId = 2;
  result = Func_08020210(target, position);
  target->terrainId = terrain;
  if (result <= 0) {
    Actor_SetMode(owner, 8);
    wait(15);
    Sound_PlayFar(185);
    target->speedLimit = 0x3333;
    target->acceleration = 0x3333;
    Actor_SetTargetPosition(target, position[0], position[1], position[2]);
    wait(2);
    acceleration = owner->acceleration;
    owner->acceleration = 0x3333;
    speed = owner->speedLimit;
    owner->speedLimit = 0x3333;
    Actor_SetTargetPosition(owner, position[0], position[1], position[2]);
    Actor_CommitPosition(target);
    wait(1);
    Event_NoOp();
    Actor_SetPositionAndResetMotionFar(target, position[0], position[1],
                                        position[2]);
    Actor_SetPositionAndResetMotionFar(owner, S16_AT(owner, 10) << 16,
                                        owner->y, S16_AT(owner, 18) << 16);
    Actor_SetMode(owner, 1);
    owner->speedLimit = speed;
    owner->acceleration = acceleration;
  }
}
