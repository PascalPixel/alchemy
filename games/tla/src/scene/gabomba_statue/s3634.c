#include "types.h"
#include "party_state.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
void Camera_MoveWithinMapBounds();
void Event_StartScreenTransition();
void Event_WaitTransitionFrames();
void Func_0200253c();
void Func_020027cc();
void Func_02002a64();
void Func_02002f70();
void Func_02003338();
s32 Func_02003604();
void Func_020048a4();
void Func_020052a4();
void Func_020053ac();
void Func_0200564c();
void Func_020056b4();
void Func_020056bc();
void Func_02005714();
void Func_0200580c();
void GameFlag_ClearBit();
s32 GameFlag_Test();
void ObjectDispatch_SetSingleChildFlags();
s32 Object_GetById();
void Object_SetModeById();
void Object_SetPartAttribute();
void Scheduler_AddOrUpdateCallback();
void WaitFrames();
void Func_02001ac0();
void Func_02002074();
extern u8 gData_02005d8c[];

void Func_02003634(void) {
  s32 t1, t4, t6, t8, t10, t12, t14, t16, t18, t20, t22, t24, t26, t27, t29,
      t30, t42, t43, t46, t50, t53, t59, t60, p_r3, v_r5;
  const u8 *table;

  t1 = Value1((s32 (*)())Object_GetById, 10);
  Call2((void (*)())Object_SetPartAttribute, t1, 3);
  Call2((void (*)())Scheduler_AddOrUpdateCallback, (s32)Func_02002074, 0x480);
  t4 = Value1((s32 (*)())Object_GetById, 11);
  Call2((void (*)())Object_SetPartAttribute, t4, 11);
  t6 = Value1((s32 (*)())Object_GetById, 12);
  Call2((void (*)())Object_SetPartAttribute, t6, 11);
  t8 = Value1((s32 (*)())Object_GetById, 13);
  Call2((void (*)())Object_SetPartAttribute, t8, 10);
  t10 = Value1((s32 (*)())Object_GetById, 14);
  Call2((void (*)())Object_SetPartAttribute, t10, 10);
  t12 = Value1((s32 (*)())Object_GetById, 16);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, t12, 0);
  t14 = Value1((s32 (*)())Object_GetById, 17);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, t14, 0);
  t16 = Value1((s32 (*)())Object_GetById, 18);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, t16, 0);
  t18 = Value1((s32 (*)())Object_GetById, 19);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, t18, 0);
  t20 = Value1((s32 (*)())Object_GetById, 20);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, t20, 0);
  t22 = Value1((s32 (*)())Object_GetById, 21);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, t22, 0);
  t24 = Value1((s32 (*)())Object_GetById, 22);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, t24, 0);
  t26 = Value1((s32 (*)())GameFlag_Test, 0x8fb);
  if (t26 == 0)
    goto L_0200b6f2;
  t27 = Value1((s32 (*)())GameFlag_Test, 0x8fd);
  if (t27 != 0)
    goto L_0200b6f2;
  Func_02002f70();
  goto L_0200b856;
L_0200b6f2:
  t29 = Value1((s32 (*)())GameFlag_Test, 0x8fc);
  if (t29 == 0)
    goto L_0200b762;
  t30 = Value1((s32 (*)())Object_GetById, gPartyState.currentOwner);
  v_r5 = t30;
  Call3((void (*)())Func_02005714, 11, 0, 0);
  Call3((void (*)())Func_02005714, 12, 0, 0);
  Call3((void (*)())Func_02005714, 13, 0, 0);
  Call3((void (*)())Func_02005714, 14, 0, 0);
  Func_020053ac();
  Func_020056b4();
  Call1((void (*)())Func_0200580c, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, FIELD_AT(v_r5, s32, 8), -1,
        FIELD_AT(v_r5, s32, 16), 0);
  Func_0200564c();
  Call1((void (*)())WaitFrames, 1);
  Func_020056bc();
L_0200b762:
  t42 = Value1((s32 (*)())GameFlag_Test, 0x8f6);
  if (t42 == 0)
    goto L_0200b78e;
  t43 = Value1((s32 (*)())GameFlag_Test, 0x8fd);
  if (t43 != 0)
    goto L_0200b78e;
  Call1((void (*)())Func_020052a4, 0);
  Call2((void (*)())Scheduler_AddOrUpdateCallback, (s32)Func_02001ac0, 0x480);
L_0200b78e:
  t46 = Value0((s32 (*)())Func_02003604);
  table = gData_02005d8c;
  Call4((void (*)())Func_020048a4, 19, 11, *(s32 *)(table + (t46 << 2)), 16);
  Call2((void (*)())Object_SetModeById, 15, 5);
  if (FIELD_AT(&gPartyState, s16, 482) != 2)
    goto L_0200b7c0;
  Func_020027cc();
  goto L_0200b856;
L_0200b7c0:
  t50 = Value1((s32 (*)())GameFlag_Test, 0x8f6);
  if (t50 != 0)
    goto L_0200b7e4;
  if (FIELD_AT(&gPartyState, s16, 482) != 1) {
    p_r3 = FIELD_AT(&gPartyState, s16, 482);
    goto L_0200b7dc;
  }
  Func_0200253c();
  goto L_0200b7e4;
L_0200b7dc:
  if (p_r3 != 3)
    goto L_0200b7e4;
  Func_02002a64();
L_0200b7e4:
  t53 = Value1((s32 (*)())GameFlag_Test, 0x8fc);
  if (t53 == 0)
    goto L_0200b856;
  Func_020056b4();
  Call1((void (*)())Func_0200580c, 0);
  FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1ac), s32, 0) = 0x201;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1ac), s32, 0) = 0x209;
  Call1((void (*)())GameFlag_ClearBit, 0x8fc);
  t59 = Value1((s32 (*)())GameFlag_Test, 0x9eb);
  if (t59 == 0)
    goto L_0200b84a;
  t60 = Value1((s32 (*)())GameFlag_Test, 0x9ec);
  if (t60 != 0)
    goto L_0200b84a;
  Func_02003338();
L_0200b84a:
  Call1((void (*)())GameFlag_ClearBit, 0x12f);
  Func_020056bc();
L_0200b856:
}
