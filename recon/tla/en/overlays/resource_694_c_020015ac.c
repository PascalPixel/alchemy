// Draft, differ 97 of 157, skeleton gap 20 (tla-en, 2026-11-01)
// A Contigo event: object 17 bursts (seventeen effects in a ring with sound 188
// and the camera shaken), turns solid again and is thrown up onto (40, 680); a
// leader standing on its cell is launched after it to (68, 680), linked to it,
// and flag 2535 is set.
// The statements are right: the second velocity is read and added to, never
// stored back (that closed 106 to 97). Left, two causes:
// 1. gcse's PRE hoists velocity's address (.07.gcse: "PRE/HOIST: edge (0,1)").
// The native builds it in the loop, add r5, sp, #16 after the cosine call, so
// its pseudo is short and the object gets r7, i sl, z then the 0x20000 r8. Here
// the hoisted pseudo takes r7, the object sl and i r8, which accounts for most
// of the differ. The goto, for, array, struct, pointer, inner-scope and named
// x and z forms all compile to the same code. The always-true branch used by
// the landing bursts (gaia_rock_inside s042c.c) is a dead device, and here it
// measured 84 only with the wrappers below, the address still hoisted. Possibly
// the compiler rather than the source: a K3 question, as in 668 and 65f.
// 2. Direct calls share constants: 0x10000 across Func_0200401c's arguments and
// 680 across both Actor_SetPositionAndCommit calls; the native builds each.
// Inline wrappers on those two give differ 84, skeleton 14/16, but are devices.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
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
    Func_02000430(object->x, object->y, object->z, velocity[0],
                  velocity[1] + 0x1999, velocity[2], 0x20000, &options);
  }
  Actor_SetPartAttribute(Actor_GetById(17), 0);
  Actor_SetSingleChildFlags(Actor_GetById(17), 1);
  Actor_SetSpeedParameters(17, 0x30000, 0x18000);
  object->velocityY = 0xc0000;
  Actor_SetPositionAndCommit(17, 40, 680);
  FieldEvent_Wait(10);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object->x >> 20 == leader->x >> 20 &&
      object->z >> 20 == leader->z >> 20) {
    Actor_Jump(gMainRoutineWork.currentOwner, 6, 0);
    Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x20000,
                                    0x10000);
    Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, 68, 680);
  }
  Func_0200401c(0, 0, 0);
  Actor_TurnToFaceEachOther(gMainRoutineWork.currentOwner, 17, 0);
  FieldEvent_Wait(10);
  GameFlag_SetBit(2535);
  Func_02004074();
}
