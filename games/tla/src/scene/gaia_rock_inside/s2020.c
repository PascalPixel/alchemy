#include "types.h"
#include "party_state.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
void FieldEvent_WaitTicks();
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
void ObjectMotion_CommitCurrentPositionAndActivate();
void ObjectMotion_OffsetPositionAndResetMotion();
void ObjectMotion_ResetAndSetPosition();
void ObjectMotion_SetAngleToward();
void ObjectMotion_SetSpeedParameters();
s32 Object_GetById();
void Object_SetModeById();
s32 Random16Far();
void UiText_ShowPositionedMessageAndWait();
void WaitTicks();
extern u8 MsgGaiaRockInside01[];

void Func_02002020(void) {
  s32 t1, t2, t3, t26, t27, t28, t34, t35, t37, t38, t43, t44, t50, v_r10, v_r5,
      v_r6, v_r7, v_r8;

  t1 = Value1((s32 (*)())Object_GetById, 8);
  v_r7 = t1;
  t2 = Value1((s32 (*)())Object_GetById, 9);
  v_r8 = t2;
  t3 = Value1((s32 (*)())Object_GetById, 13);
  v_r10 = t3;
  Func_02004e5c();
  Call1((void (*)())Func_02004fcc, 0);
  Call2((void (*)())UiText_ShowPositionedMessageAndWait,
        (s32)&MsgGaiaRockInside01, 1);
  Call1((void (*)())Func_02005014, 193);
  Call2((void (*)())Func_02004e3c, v_r10, 1);
  v_r5 = (s32)&gPartyState;
  v_r5 = (v_r5 + 0x214);
  Call2((void (*)())Func_02004ec4, 13, FIELD_AT(v_r5, s32, 0));
  Call3((void (*)())ObjectMotion_ResetAndSetPosition, 13, 0x1f0, 0x2c8);
  FIELD_AT(v_r10, s32, 40) = 0x60000;
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 13);
  Call1((void (*)())FieldEvent_WaitTicks, 45);
  Call3((void (*)())ObjectMotion_SetAngleToward, 8, FIELD_AT(v_r5, s32, 0), 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 9, FIELD_AT(v_r5, s32, 0), 0);
  Call1((void (*)())Func_02005014, 0x240);
  Call2((void (*)())Object_SetModeById, 8, 5);
  Call2((void (*)())Object_SetModeById, 9, 5);
  Call1((void (*)())WaitTicks, 30);
  Call2((void (*)())Object_SetModeById, 8, 2);
  Call2((void (*)())Object_SetModeById, 9, 2);
  Call1((void (*)())FieldEvent_WaitTicks, 30);
  Call2((void (*)())Object_SetModeById, 8, 3);
  Call2((void (*)())Object_SetModeById, 9, 3);
  Call1((void (*)())FieldEvent_WaitTicks, 40);
  Call1((void (*)())Func_02005014, 191);
  t26 = Value1((s32 (*)())GameFlag_Test, 0xa62);
  v_r5 = 119;
  if (t26 != 0)
    goto L_0200a1d8;
  v_r5 = 19;
L_0200a10a:
  t27 = Value0((s32 (*)())Random16Far);
  FIELD_AT(v_r7, s32, 40) = t27;
  t28 = Value0((s32 (*)())Random16Far);
  FIELD_AT(v_r8, s32, 40) = t28;
  v_r5 = (v_r5 + -1);
  Call1((void (*)())WaitTicks, 1);
  if (v_r5 >= 0)
    goto L_0200a10a;
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 8, 0x4000, 0xccc);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 9, 0x4000, 0xccc);
  Call3((void (*)())ObjectMotion_OffsetPositionAndResetMotion, 8, 24, 0);
  Call3((void (*)())ObjectMotion_OffsetPositionAndResetMotion, 9, -24, 0);
  v_r6 = v_r7;
  v_r6 = (v_r6 + 86);
  v_r5 = 0;
  for (; v_r5 < 300 && FIELD_AT(v_r6, u8, 0) != 0; v_r5++) {
    t34 = Value0((s32 (*)())Random16Far);
    FIELD_AT(v_r7, s32, 40) = t34;
    t35 = Value0((s32 (*)())Random16Far);
    FIELD_AT(v_r8, s32, 40) = t35;
    Call1((void (*)())WaitTicks, 1);
  }
  v_r5 = 9;
L_0200a18c:
  t37 = Value0((s32 (*)())Random16Far);
  FIELD_AT(v_r7, s32, 40) = t37;
  t38 = Value0((s32 (*)())Random16Far);
  FIELD_AT(v_r8, s32, 40) = t38;
  v_r5 = (v_r5 + -1);
  Call1((void (*)())WaitTicks, 1);
  if (v_r5 >= 0)
    goto L_0200a18c;
  v_r5 = 40;
  Call6((void (*)())Func_02004df4, 119, 31, 1, 2, 89, v_r5);
  Call6((void (*)())Func_02004df4, 119, 31, 1, 2, 100, v_r5);
  Call1((void (*)())GameFlag_SetBit, 0xa62);
  goto L_0200a1f8;
L_0200a1d8:
  t43 = Value0((s32 (*)())Random16Far);
  FIELD_AT(v_r7, s32, 40) = t43;
  t44 = Value0((s32 (*)())Random16Far);
  FIELD_AT(v_r8, s32, 40) = t44;
  v_r5 = (v_r5 + -1);
  Call1((void (*)())WaitTicks, 1);
  if (v_r5 >= 0)
    goto L_0200a1d8;
  Call1((void (*)())FieldEvent_WaitTicks, 30);
L_0200a1f8:
  Call2((void (*)())Object_SetModeById, 8, 4);
  Call2((void (*)())Object_SetModeById, 9, 4);
  Call1((void (*)())FieldEvent_WaitTicks, 30);
  t50 = Value1((s32 (*)())Object_GetById, gPartyState.currentOwner);
  if (t50 == 0)
    goto L_0200a22e;
  Call3((void (*)())ObjectMotion_ResetAndSetPosition, 13,
        FIELD_AT(t50, s16, 10), FIELD_AT(t50, s16, 18));
L_0200a22e:
  FIELD_AT(v_r10, s32, 40) = 0x60000;
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 13);
  Call3((void (*)())Func_02004ebc, 13, 0, 0);
  Func_02004e64();
}
