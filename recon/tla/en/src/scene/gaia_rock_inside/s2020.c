#include "types.h"
#include "main_routine_work.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
void FieldEvent_Wait();
void Func_02004df4();
void Func_02004e3c();
void Func_02004e5c();
void Func_02004e64();
void Func_02004ebc();
void Func_02004ec4();
void Func_02004fcc();
void Func_02005014();
void GameFlag_SetBit();
s32 GameFlag_Test();
void Actor_WaitForMoveById();
void Actor_OffsetPositionAndResetMotion();
void Actor_ResetAndSetPosition();
void Actor_SetAngleToward();
void Actor_SetSpeedParameters();
s32 Actor_GetById();
void Actor_SetModeById();
s32 Random16Far();
void FieldMessage_Show();
void wait();
extern u8 MsgGaiaRockInside01[];

void Func_02002020(void) {
  s32 t1, t2, t3, t26, t27, t28, t34, t35, t37, t38, t43, t44, t50, v_r10, v_r5,
      v_r6, v_r7, v_r8;

  t1 = Actor_GetById(8);
  v_r7 = t1;
  t2 = Actor_GetById(9);
  v_r8 = t2;
  t3 = Actor_GetById(13);
  v_r10 = t3;
  Func_02004e5c();
  Func_02004fcc(0);
  FieldMessage_Show((s32)&MsgGaiaRockInside01, 1);
  Func_02005014(193);
  Func_02004e3c(v_r10, 1);
  v_r5 = (s32)&gMainRoutineWork;
  v_r5 = (v_r5 + 0x214);
  Func_02004ec4(13, FIELD_AT(v_r5, s32, 0));
  Call3((void (*)())Actor_ResetAndSetPosition, 13, 496, 712);
  FIELD_AT(v_r10, s32, 40) = 0x60000;
  Actor_WaitForMoveById(13);
  FieldEvent_Wait(45);
  Actor_SetAngleToward(8, FIELD_AT(v_r5, s32, 0), 0);
  Actor_SetAngleToward(9, FIELD_AT(v_r5, s32, 0), 0);
  Func_02005014(0x240);
  Actor_SetModeById(8, 5);
  Actor_SetModeById(9, 5);
  wait(30);
  Actor_SetModeById(8, 2);
  Actor_SetModeById(9, 2);
  FieldEvent_Wait(30);
  Actor_SetModeById(8, 3);
  Actor_SetModeById(9, 3);
  FieldEvent_Wait(40);
  Func_02005014(191);
  t26 = GameFlag_Test(0xa62);
  v_r5 = 119;
  if (t26 != 0)
    goto L_0200a1d8;
  v_r5 = 19;
L_0200a10a:
  t27 = Random16Far();
  FIELD_AT(v_r7, s32, 40) = t27;
  t28 = Random16Far();
  FIELD_AT(v_r8, s32, 40) = t28;
  v_r5 = (v_r5 + -1);
  wait(1);
  if (v_r5 >= 0)
    goto L_0200a10a;
  Call3((void (*)())Actor_SetSpeedParameters, 8, 0x4000, 0xccc);
  Call3((void (*)())Actor_SetSpeedParameters, 9, 0x4000, 0xccc);
  Actor_OffsetPositionAndResetMotion(8, 24, 0);
  Call3((void (*)())Actor_OffsetPositionAndResetMotion, 9, -24, 0);
  v_r6 = v_r7;
  v_r6 = (v_r6 + 86);
  v_r5 = 0;
  for (; v_r5 < 300 && FIELD_AT(v_r6, u8, 0) != 0; v_r5++) {
    t34 = Random16Far();
    FIELD_AT(v_r7, s32, 40) = t34;
    t35 = Random16Far();
    FIELD_AT(v_r8, s32, 40) = t35;
    wait(1);
  }
  v_r5 = 9;
L_0200a18c:
  t37 = Random16Far();
  FIELD_AT(v_r7, s32, 40) = t37;
  t38 = Random16Far();
  FIELD_AT(v_r8, s32, 40) = t38;
  v_r5 = (v_r5 + -1);
  wait(1);
  if (v_r5 >= 0)
    goto L_0200a18c;
  v_r5 = 40;
  Func_02004df4(119, 31, 1, 2, 89, v_r5);
  Func_02004df4(119, 31, 1, 2, 100, v_r5);
  GameFlag_SetBit(0xa62);
  goto L_0200a1f8;
L_0200a1d8:
  t43 = Random16Far();
  FIELD_AT(v_r7, s32, 40) = t43;
  t44 = Random16Far();
  FIELD_AT(v_r8, s32, 40) = t44;
  v_r5 = (v_r5 + -1);
  wait(1);
  if (v_r5 >= 0)
    goto L_0200a1d8;
  FieldEvent_Wait(30);
L_0200a1f8:
  Actor_SetModeById(8, 4);
  Actor_SetModeById(9, 4);
  FieldEvent_Wait(30);
  t50 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (t50 == 0)
    goto L_0200a22e;
  Actor_ResetAndSetPosition(13, FIELD_AT(t50, s16, 10), FIELD_AT(t50, s16, 18));
L_0200a22e:
  FIELD_AT(v_r10, s32, 40) = 0x60000;
  Actor_WaitForMoveById(13);
  Func_02004ebc(13, 0, 0);
  Func_02004e64();
}
