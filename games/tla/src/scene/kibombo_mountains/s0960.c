#include "types.h"
#include "main_routine_work.h"
#include "call.h"

extern u8 MsgIsTheWay[];
extern const u8 gData_020017f8[];

void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
void GameFlag_SetBit(s32 flag);
void Camera_MoveWithinMapBounds(s32 a, s32 b, s32 c, s32 d);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Motion_SetVarCbAndRefresh(s32 actor, s32 value);
void Actor_WatchAndSetScript(s32 actor, s32 target);
void Actor_ResetTargetsAndVelocity(s32 actor);
void Actor_SetAngleToward(s32 actor, s32 target, s32 frames);
void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Actor_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);
void Actor_SetMovementTypeById(s32 actor, const u8 *action);
void Func_02001084(void);
void Func_0200108c(void);
void Func_020010d4(s32 actor, s32 x, s32 z);
void Func_02001104(const u8 *message);
void Func_0200110c(s32 actor, s32 flags);
void Func_0200111c(s32 actor, s32 a, s32 b);
void Func_02001134(void);
void Func_02001164(s32 value);

void Func_02000960(void) {
  /* FAKEMATCH: the Call3 and Value3 calls of call.h keep each call's own
   * constants and argument order; as direct SetPositionAndReset calls, 42
   * instructions differ in tla-en. */
  Func_02001084();
  Func_02001164(0);
  Func_02001104(MsgIsTheWay);
  Call3((void (*)())Func_020010d4, 27, 0x1380000, 0x1300000);
  Func_0200110c(27, 0);
  Camera_MoveWithinMapBounds(0x1000000, -1, 0x1880000, 1);
  Func_02001134();
  Actor_ResetTargetsAndVelocity(11);
  Actor_SetAngleToward(11, 27, 0);
  Call3((void (*)())Actor_SetPositionAndReset, 27, 264, 312);
  Call3((void (*)())Actor_SetPositionAndReset, 27, 256, 400);
  Motion_SetModeAndWaitAnimation(11, 3);
  Func_0200110c(11, 0);
  Motion_SetVarCbAndRefresh(11, 1);
  Func_0200110c(11, 0);
  Motion_SetModeAndWaitAnimation(27, 4);
  Func_0200110c(27, 0);
  Func_0200110c(27, 0);
  Value3((s32 (*)())Func_0200111c, 11, 258, 30);
  Func_0200110c(11, 0);
  Value3((s32 (*)())Func_0200111c, 27, 256, 30);
  Func_0200110c(27, 0);
  Motion_SetModeAndWaitAnimation(11, 3);
  Func_0200110c(11, 0);
  Value3((s32 (*)())Func_0200111c, 27, 258, 30);
  Func_0200110c(27, 0);
  Call3((void (*)())Actor_SetPositionAndReset, 27, 264, 312);
  Call3((void (*)())Actor_ResetAndSetPositionInMode2, 27, 360, 312);
  Motion_SetModeAndWaitAnimation(11, 3);
  Func_0200110c(11, 0);
  Actor_WatchAndSetScript(11, gMainRoutineWork.currentOwner);
  wait(30);
  Func_0200110c(11, 0);
  Motion_SetVarCbAndRefresh(11, 2);
  FieldEvent_Wait(60);
  Call3((void (*)())Func_020010d4, 27, 0, 0);
  Actor_SetMovementTypeById(11, gData_020017f8);
  Func_0200108c();
  GameFlag_SetBit(0x880);
}
