#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"

void Func_0200406c(void);
void Func_02004074(void);
s32 Math_Cosine(s32 angle);
void Func_02000430(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);

void Func_020015ac(void) {
  struct Actor *object = Actor_GetById(17);
  struct Actor *leader;
  struct EffectOptions options;
  s32 velocity[3];
  s32 angle;
  u32 i;

  Func_0200406c();
  Event_ResolvePendingActions(0);
  Actor_SetHorizontalPositionWithTerrain(17, 0x380000, 0x2ba0000);
  wait(1);
  options.priority = 2;
  Sound_Play(188);
  MapGrid_SetShake(0x10000, 0x20000, 0x10000);
  for (i = 0; i <= 16; i++) {
    angle = i << 12;
    velocity[0] = Math_Cosine(angle);
    velocity[1] = 0;
    velocity[2] = Math_Sine(angle);
    velocity[0] += velocity[0] / 3;
    Func_02000430(object->x, object->y, object->z, velocity[0],
                  velocity[1] + 0x1999, velocity[2], 0x20000, &options);
  }
  Actor_SetPartAttribute(Actor_GetById(17), 0);
  Actor_SetSingleChildFlags(Actor_GetById(17), 1);
  Actor_SetSpeed(17, 0x30000, 0x18000);
  object->velocityY = 0xc0000;
  Actor_MoveToAndWait(17, 40, 680);
  Event_Wait(10);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object->x >> 20 == leader->x >> 20 &&
      object->z >> 20 == leader->z >> 20) {
    Actor_Jump(gMainRoutineWork.currentOwner, 6, 0);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x20000, 0x10000);
    Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 68, 680);
  }
  Map_SetScreenShake(0, 0, 0);
  Actor_TurnToFaceEachOther(gMainRoutineWork.currentOwner, 17, 0);
  Event_Wait(10);
  GameFlag_SetBit(2535);
  Func_02004074();
}
