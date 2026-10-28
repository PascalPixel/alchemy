// Draft, differ 106 of 157: a Contigo event: object 17 bursts (seventeen effects in
// a ring with sound 188 and the camera shaken), turns solid again and is thrown
// up onto (40, 680); a leader standing on its cell is launched after it to
// (68, 680), linked to it, and flag 2535 is set.
#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "field_effect.h"

struct Actor *Actor_GetById(s32 id);
void Func_0200406c(void);
void Func_02004074(void);
void Func_02004204(s32 value);
void Func_020040d4(s32 id, s32 a, s32 b);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
void Func_020042b4(s32 sound);
void Func_0200401c(s32 a, s32 b, s32 c);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Func_02000430(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetPositionAndCommit(s32 id, s32 x, s32 z);
void Actor_Jump(s32 id, s32 mode, s32 value);
void Actor_TurnToFaceEachOther(s32 id, s32 other, s32 value);
void GameFlag_SetBit(s32 flag);

void Func_020015ac(void) {
  struct Actor *object = Actor_GetById(17);
  struct Actor *leader;
  struct EffectOptions options;
  s32 velocity[3];
  s32 angle;
  u32 i;

  Func_0200406c();
  Func_02004204(0);
  Func_020040d4(17, 0x380000, 0x2ba0000);
  wait(1);
  options.priority = 2;
  Func_020042b4(188);
  Func_0200401c(0x10000, 0x20000, 0x10000);
  for (i = 0; i <= 16; i++) {
    angle = i << 12;
    velocity[0] = Math_Cosine(angle);
    velocity[1] = 0;
    velocity[2] = Math_Sine(angle);
    velocity[0] += velocity[0] / 3;
    velocity[1] += 0x1999;
    Func_02000430(object->x, object->y, object->z, velocity[0], velocity[1],
                  velocity[2], 0x20000, &options);
  }
  Actor_SetPartAttribute(Actor_GetById(17), 0);
  Actor_SetSingleChildFlags(Actor_GetById(17), 1);
  Actor_SetSpeedParameters(17, 0x30000, 0x18000);
  object->velocityY = 0xc0000;
  Actor_SetPositionAndCommit(17, 40, 680);
  FieldEvent_Wait(10);
  leader = Actor_GetById(gSequenceParentWork.currentOwner);
  if (object->x >> 20 == leader->x >> 20 &&
      object->z >> 20 == leader->z >> 20) {
    Actor_Jump(gSequenceParentWork.currentOwner, 6, 0);
    Actor_SetSpeedParameters(gSequenceParentWork.currentOwner, 0x20000,
                                    0x10000);
    Actor_SetPositionAndCommit(gSequenceParentWork.currentOwner, 68, 680);
  }
  Func_0200401c(0, 0, 0);
  Actor_TurnToFaceEachOther(gSequenceParentWork.currentOwner, 17, 0);
  FieldEvent_Wait(10);
  GameFlag_SetBit(2535);
  Func_02004074();
}
