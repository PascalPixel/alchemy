// Draft, differ 142 of 1485 (was 222): Garoh's long scene with actors 4 to 9
// after MsgGaroh01, with two bird-path loops on Math_Sine. The paths step
// through Garoh_PathStep, so the native's multiply-then-divide survives (a
// plain expression folds it to one multiply); each Math_Sine result is taken
// into a variable first, so the step is computed before the amplitude; the
// second loop's targets are variables, as the native keeps 0x800000 and
// 0xb80000 in r9 and sl. The message decrement goes through s16, which gives
// the native's subs #1 (u16 builds 0xffff). Left: the position array's address
// (and the second loop's target differences), which this compiler's PRE hoists
// out of each loop and the native forms inside; register choice that follows
// from it; and argument order at about 20 calls, where sched2 issues the last
// argument first (a direct call and a CallN wrapper make no difference there).
#include "../../../../games/tla/src/scene/garoh/garoh.h"
#include "adventure.h"
#include "event_work.h"
#include "call.h"
void Func_020013b0(s32 id);
#define FOLLOW_LEADER(id)                                                      \
  {                                                                            \
    struct Actor *leader_;                                             \
                                                                               \
    Actor_SetSpeedParameters(id, 0x13333, 0x9999);                      \
    Actor_SetModeById(id, 2);                                                 \
    leader_ = Actor_GetById(gAdventure.currentOwner);                        \
    if (leader_ != NULL)                                                       \
      Actor_ResetAndSetPosition(id, *(s16 *)((u8 *)leader_ + 10),       \
                                       *(s16 *)((u8 *)leader_ + 18));          \
    Actor_WaitForMoveById(id);                         \
    Func_0200520c(id, 0, 0);                                                   \
  }

extern u8 MsgGaroh01[];
extern const u8 gData_0200545c[];

void GameFlag_SetBit(s32 flag);
s32 GameFlag_Test(s32 flag);
s32 Math_Sine(s32 angle);
u32 Random16Far(void);
void Func_0200520c(s32 id, s32 x, s32 z);
void Func_02005274(s32 a, s32 b);
void Func_0200528c(s32 id, s32 animation, s32 frames);
void Func_02005294(s32 id, s32 animation);
void Func_020052b4(void);
void Func_02005334(s32 id, s32 x, s32 z, s32 angle);
void Func_02005324(s32 a);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Actor_ResetAndSetPosition(s32 id, s32 x, s32 z);
void Actor_CommitPositionAndActivate(s32 id, s32 x, s32 z);
void Actor_WaitForMoveById(s32 id);
void Actor_ArmCallback(s32 id, s32 angle, s32 frames);
void Actor_StartRepeatedMotion(s32 id, s32 value);
void Actor_Jump(s32 id, s32 height, s32 frames);
void Actor_SetMovementTypeById(s32 id, const void *callback);
void Actor_Stop(s32 id);
void Motion_SetVarCbAndRefresh(s32 id, s32 value);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Camera_FollowActor(s32 id, s32 mode);

static inline s32 Garoh_PathStep(s32 from, s32 to, s32 i) {
  return from + ((to - from) * i) / 8;
}

void Func_02001490(void) {
  struct Actor *bird;
  s32 i;
  s32 position[3];
  s32 x, z;
  s32 toX, toZ;

  Call1((void (*)())GameFlag_SetBit, 0x91c);
  Func_020051ac();
  Call1((void (*)())Func_02005324, 0);
  Call3((void (*)())Func_0200520c, 8, 0x600000, 0x80000);
  Func_0200524c((s32)MsgGaroh01);
  Call3((void (*)())Func_0200525c, 8, 0, 0);
  Value3((s32 (*)())Func_0200528c, 4, 0x100, 40);
  Call3((void (*)())Actor_SetSpeedParameters, 4, 0xcccc, 0x6666);
  Call3((void (*)())Actor_SetPositionAndReset, 4, 152, 304);
  Call4((void (*)())Func_02005334, 9, 32, -16, 0xb000);
  Call4((void (*)())Func_02005334, 5, 0, 0, 0xc000);
  Call4((void (*)())Func_02005334, 6, 16, 0, 0xb000);
  if (GameFlag_Test(7))
    Call4((void (*)())Func_02005334, 7, 8, 16, 0xc000);
  Call3((void (*)())Actor_CommitPositionAndActivate, 4, -16, -16);
  Call3((void (*)())Actor_ArmCallback, 4, 0xc000, 0);
  Call1((void (*)())Actor_WaitForMoveById, 9);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Func_0200525c, 8, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 4, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 5, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 6, 0x8000, 0);
  Call3((void (*)())Actor_ArmCallback, 9, 0x8000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 25);
  Call3((void (*)())Func_0200525c, 5, 0, 0);
  Value3((s32 (*)())Func_0200528c, 6, 0x101, 0);
  Value3((s32 (*)())Func_0200528c, 9, 0x101, 0);
  if (GameFlag_Test(7))
    Value3((s32 (*)())Func_0200528c, 7, 0x101, 0);
  Value3((s32 (*)())Func_0200528c, 4, 0x101, 50);
  Call3((void (*)())Func_0200525c, 8, 0, 0);
  if (GameFlag_Test(7))
    Call2((void (*)())Actor_StartRepeatedMotion, 7, 2);
  Call2((void (*)())Actor_StartRepeatedMotion, 6, 2);
  Call2((void (*)())Actor_StartRepeatedMotion, 9, 2);
  Call2((void (*)())Actor_StartRepeatedMotion, 4, 2);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 5, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Actor_ArmCallback, 4, 0xc000, 0);
  Call3((void (*)())Actor_ArmCallback, 5, 0xc000, 0);
  Call3((void (*)())Actor_ArmCallback, 6, 0xc000, 0);
  Call3((void (*)())Actor_ArmCallback, 9, 0xb000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x900000, -1, 0x800000, 1);
  Func_020052b4();
  bird = Actor_GetById(8);
  Call2((void (*)())Actor_SetModeById, 8, 2);
  i = 0;
loop1:
  {
    s32 r;
    s32 wave;

    wave = Math_Sine(i << 13);
    position[0] = Garoh_PathStep(0x680000, 0x580000, i) - wave * 12;
    wave = Math_Sine(i << 13);
    position[2] = Garoh_PathStep(0x200000, 0x780000, i) - wave * 7;
    position[1] = 0;
    r = ((Random16Far() * 10) >> 16) + 4;
    Call3((void (*)())Actor_SetSpeedParameters, 8, r * 0x1999,
          r * 0xccc);
    Actor_SetPositionAndReset(8, position[0] / 0x10000,
                                     position[2] / 0x10000);
    i++;
    if (i <= 7)
      goto loop1;
  }
  Call2((void (*)())Actor_SetModeById, 8, 1);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 8, 2);
  Actor_SetMovementTypeById(8, gData_0200545c);
  (*(s16 *)&((struct EventWork *)Ram_HeapSlots->eventWork)->messageId)--;
  Call3((void (*)())Func_0200525c, 8, 0, 0);
  Call1((void (*)())Actor_Stop, 8);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Actor_ArmCallback, 8, 0x2000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call2((void (*)())Camera_FollowActor, 8, 1);
  Call2((void (*)())Actor_SetModeById, 8, 2);
  x = bird->x;
  z = bird->z;
  toX = 0x800000;
  toZ = 0xb80000;
  i = 0;
loop2:
  {
    s32 r;
    s32 wave;

    wave = Math_Sine(i << 13);
    position[0] = Garoh_PathStep(x, toX, i) - wave * 4;
    position[2] = Garoh_PathStep(z, toZ, i);
    position[1] = 0;
    r = ((Random16Far() * 10) >> 16) + 4;
    Call3((void (*)())Actor_SetSpeedParameters, 8, r * 0x1999,
          r * 0xccc);
    Actor_SetPositionAndReset(8, position[0] / 0x10000,
                                     position[2] / 0x10000);
    i++;
    if (i <= 7)
      goto loop2;
  }
  Call2((void (*)())Actor_SetModeById, 8, 1);
  Call3((void (*)())Actor_ArmCallback, 8, 0x3000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Actor_ArmCallback, 8, 0, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())Func_0200525c, 8, 0, 0);
  Call3((void (*)())Func_0200525c, 6, 0, 0);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 8, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Actor_ArmCallback, 8, 0x3000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Value3((s32 (*)())Func_0200528c, 8, 0x102, 50);
  Call3((void (*)())Actor_SetSpeedParameters, 8, 0x9999, 0x4ccc);
  Actor_GetById(8)->actionFlags &= 254;
  Call3((void (*)())Actor_SetPositionAndReset, 8, 128, 148);
  Call1((void (*)())FieldEvent_WaitFrames, 1);
  Actor_GetById(8)->actionFlags |= 1;
  Call3((void (*)())Actor_SetSpeedParameters, 8, 0x28000, 0x14000);
  Call3((void (*)())Actor_CommitPositionAndActivate, 8, 0, 4);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Call3((void (*)())Actor_ArmCallback, 8, 0xc000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call2((void (*)())Actor_StartRepeatedMotion, 8, 3);
  Call2((void (*)())Func_02005294, 8, 0x102);
  Call1((void (*)())FieldEvent_WaitFrames, 40);
  Call3((void (*)())Actor_ArmCallback, 8, 0x8000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Actor_SetSpeedParameters, 8, 0x26666, 0x13333);
  Call3((void (*)())Actor_CommitPositionAndActivate, 8, -24, -8);
  Call3((void (*)())Actor_CommitPositionAndActivate, 8, -8, -100);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())Func_0200520c, 8, 0, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0xa80000, -1, 0x1180000, 1);
  Func_020052b4();
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Value3((s32 (*)())Func_0200528c, 4, 0x102, 0);
  Value3((s32 (*)())Func_0200528c, 5, 0x102, 0);
  if (GameFlag_Test(7))
    Value3((s32 (*)())Func_0200528c, 7, 0x102, 0);
  Value3((s32 (*)())Func_0200528c, 6, 0x102, 40);
  Call3((void (*)())Actor_Jump, 9, 6, 15);
  Call3((void (*)())Actor_Jump, 9, 6, 23);
  Call2((void (*)())Actor_SetModeById, 9, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_0200525c, 9, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 5, 0xe000, 0);
  Call3((void (*)())Actor_ArmCallback, 6, 0xe000, 0);
  Call3((void (*)())Actor_ArmCallback, 4, 0, 0);
  if (GameFlag_Test(7))
    Call3((void (*)())Actor_ArmCallback, 7, 0xd000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 25);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 5, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_0200525c, 5, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 9, 0x5000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 25);
  Call3((void (*)())Func_0200525c, 9, 0, 0);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 6, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_0200525c, 0x2006, 0, 0);
  Value3((s32 (*)())Func_0200528c, 9, 0x108, 30);
  Call3((void (*)())Func_0200525c, 9, 0, 0);
  if (GameFlag_Test(7))
    Value3((s32 (*)())Func_0200528c, 7, 0x101, 0);
  Value3((s32 (*)())Func_0200528c, 6, 0x101, 0);
  Value3((s32 (*)())Func_0200528c, 5, 0x101, 0);
  Value3((s32 (*)())Func_0200528c, 4, 0x101, 50);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 5, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_0200525c, 5, 0, 0);
  Call2((void (*)())Actor_SetModeById, 9, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_0200525c, 9, 0, 0);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 6, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_0200525c, 0x2006, 0, 0);
  Value3((s32 (*)())Func_0200528c, 9, 0x100, 30);
  Call3((void (*)())Func_0200525c, 9, 0, 0);
  Value3((s32 (*)())Func_0200528c, 6, 0x101, 50);
  Call2((void (*)())Actor_SetModeById, 9, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_0200525c, 9, 0, 0);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 9, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_0200525c, 9, 0, 0);
  Call2((void (*)())Actor_SetModeById, 5, 3);
  Call2((void (*)())Actor_SetModeById, 6, 3);
  if (GameFlag_Test(7))
    Call2((void (*)())Actor_SetModeById, 7, 3);
  Call2((void (*)())Actor_SetModeById, 4, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Value3((s32 (*)())Func_0200528c, 9, 0x109, 30);
  Call3((void (*)())Func_0200525c, 9, 0, 0);
  Call2((void (*)())Actor_SetModeById, 9, 4);
  Call1((void (*)())FieldEvent_WaitFrames, 33);
  Call3((void (*)())Func_0200525c, 9, 0, 0);
  Call3((void (*)())Actor_CommitPositionAndActivate, 9, -8, -8);
  Call3((void (*)())Actor_ArmCallback, 6, 0xc000, 0);
  Call3((void (*)())Actor_CommitPositionAndActivate, 9, -10, -6);
  Call3((void (*)())Actor_ArmCallback, 5, 0xc000, 0);
  Call3((void (*)())Actor_CommitPositionAndActivate, 9, -14, -4);
  Call3((void (*)())Actor_ArmCallback, 4, 0xe000, 0);
  Call3((void (*)())Actor_ArmCallback, 6, 0xa000, 0);
  if (GameFlag_Test(7))
    Call3((void (*)())Actor_ArmCallback, 7, 0xc000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())Actor_ArmCallback, 9, 0xc000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call2((void (*)())Actor_SetModeById, 9, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_0200525c, 0x2009, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 5, 0xa000, 0);
  if (GameFlag_Test(7))
    Call3((void (*)())Actor_ArmCallback, 7, 0xb000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 0x2005, 0);
  Call3((void (*)())Actor_ArmCallback, 4, 0x2000, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Call1((void (*)())FieldEvent_WaitFrames, 30);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 9, 2);
    Call1((void (*)())FieldEvent_WaitFrames, 20);
    Call3((void (*)())Actor_ArmCallback, 9, 0x5000, 0);
    Call1((void (*)())FieldEvent_WaitFrames, 20);
    if (GameFlag_Test(7))
      Call3((void (*)())Actor_ArmCallback, 7, 0xc000, 0);
    Call3((void (*)())Func_0200525c, 0x2009, 0, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
  } else {
    Call1((void (*)())FieldEvent_WaitFrames, 40);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 9, 2);
    Call1((void (*)())FieldEvent_WaitFrames, 20);
    Call3((void (*)())Actor_ArmCallback, 9, 0x5000, 0);
    Call1((void (*)())FieldEvent_WaitFrames, 20);
    if (GameFlag_Test(7))
      Call3((void (*)())Actor_ArmCallback, 7, 0xc000, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
    Call3((void (*)())Func_0200525c, 0x2009, 0, 0);
  }
  Call3((void (*)())Actor_Jump, 9, 6, 15);
  Call3((void (*)())Actor_Jump, 9, 6, 23);
  Call3((void (*)())Func_0200525c, 0x2009, 0, 0);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 5, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Actor_ArmCallback, 5, 0xc000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 15);
  Call3((void (*)())Func_0200525c, 5, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 4, 0xe000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 25);
  Call2((void (*)())Actor_SetModeById, 9, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call1((void (*)())FieldEvent_WaitFrames, 8);
  Call2((void (*)())Actor_SetModeById, 9, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call1((void (*)())FieldEvent_WaitFrames, 8);
  Call2((void (*)())Actor_SetModeById, 9, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Value3((s32 (*)())Func_0200528c, 6, 0x101, 30);
  Call3((void (*)())Func_0200525c, 0x2006, 0, 0);
  Call2((void (*)())Actor_StartRepeatedMotion, 9, 3);
  Call2((void (*)())Func_02005294, 9, 0x102);
  Call1((void (*)())FieldEvent_WaitFrames, 40);
  Call3((void (*)())Func_0200525c, 0x2009, 0, 0);
  Call1((void (*)())Func_020013b0, 6);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Func_0200525c, 0x2006, 0, 0);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 9, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Actor_SetSpeedParameters, 9, 0x6666, 0x3333);
  Actor_GetById(9)->actionFlags &= 254;
  Call3((void (*)())Actor_CommitPositionAndActivate, 9, 0, -16);
  Call1((void (*)())FieldEvent_WaitFrames, 1);
  Actor_GetById(9)->actionFlags |= 1;
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call2((void (*)())Func_02005294, 9, 0x102);
  Call1((void (*)())FieldEvent_WaitFrames, 40);
  Call3((void (*)())Func_0200525c, 0x2009, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 5, 0xa000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Func_0200525c, 5, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 6, 0x6000, 0);
  Call3((void (*)())Actor_ArmCallback, 4, 0x2000, 0);
  if (GameFlag_Test(7))
    Call3((void (*)())Actor_ArmCallback, 7, 0xb000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call2((void (*)())Actor_SetModeById, 6, 3);
  if (GameFlag_Test(7))
    Call2((void (*)())Actor_SetModeById, 7, 3);
  Call2((void (*)())Actor_SetModeById, 4, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call1((void (*)())FieldEvent_WaitFrames, 35);
  if (GameFlag_Test(7))
    Call3((void (*)())Actor_ArmCallback, 7, 0xc000, 0);
  Call3((void (*)())Actor_ArmCallback, 5, 0xc000, 0);
  Call3((void (*)())Actor_ArmCallback, 6, 0xa000, 0);
  Call3((void (*)())Actor_ArmCallback, 4, 0xe000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 25);
  Value3((s32 (*)())Func_0200528c, 9, 0x109, 30);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Actor_SetSpeedParameters, 9, 0x10000, 0x8000);
  Call3((void (*)())Actor_CommitPositionAndActivate, 9, 0, 16);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Value3((s32 (*)())Func_0200528c, 9, 0x103, 30);
  Call3((void (*)())Func_0200525c, 0x2009, 0, 0);
  Value3((s32 (*)())Func_0200528c, 4, 0x100, 0);
  Value3((s32 (*)())Func_0200528c, 5, 0x100, 0);
  if (GameFlag_Test(7))
    Value3((s32 (*)())Func_0200528c, 7, 0x100, 0);
  Value3((s32 (*)())Func_0200528c, 6, 0x100, 50);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 9, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_0200525c, 0x2009, 0, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 9, 0x20000, 0x10000);
  Call3((void (*)())Actor_CommitPositionAndActivate, 9, -16, 0);
  Call3((void (*)())Actor_ArmCallback, 5, 0xa000, 0);
  Call3((void (*)())Actor_ArmCallback, 6, 0xa000, 0);
  Call3((void (*)())Actor_ArmCallback, 4, 0xc000, 0);
  Call3((void (*)())Actor_ArmCallback, 9, 0x4000, 0);
  if (GameFlag_Test(7))
    Call3((void (*)())Actor_ArmCallback, 7, 0xa000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Value3((s32 (*)())Func_0200528c, 4, 0x102, 40);
  Call2((void (*)())Actor_SetModeById, 9, 4);
  Call1((void (*)())FieldEvent_WaitFrames, 33);
  Call3((void (*)())Func_0200525c, 0x2009, 0, 0);
  Call2((void (*)())Actor_SetModeById, 9, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Func_0200525c, 0x2009, 0, 0);
  Call3((void (*)())Actor_Jump, 9, 6, 15);
  Call3((void (*)())Actor_Jump, 9, 6, 23);
  if (GameFlag_Test(7))
    Value3((s32 (*)())Func_0200528c, 7, 0x102, 0);
  Value3((s32 (*)())Func_0200528c, 6, 0x102, 0);
  Value3((s32 (*)())Func_0200528c, 4, 0x102, 0);
  Value3((s32 (*)())Func_0200528c, 5, 0x102, 50);
  Call1((void (*)())FieldEvent_WaitFrames, 40);
  Call2((void (*)())Func_02005274, 4, 0x2000);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call2((void (*)())Actor_SetModeById, 6, 3);
  Call2((void (*)())Actor_SetModeById, 4, 3);
  if (GameFlag_Test(7))
    Call2((void (*)())Actor_SetModeById, 7, 3);
  Call2((void (*)())Actor_SetModeById, 5, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  FOLLOW_LEADER(9);
  FOLLOW_LEADER(5);
  FOLLOW_LEADER(6);
  if (GameFlag_Test(7)) {
    FOLLOW_LEADER(7);
  }
  Call2((void (*)())Camera_FollowActor, gAdventure.currentOwner, 1);
  Func_020051b4();
}
