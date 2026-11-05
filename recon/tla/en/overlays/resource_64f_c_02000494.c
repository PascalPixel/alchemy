// Draft of overlay 64f's Func_02000494, translated from its listing.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_02000988();
s32 Func_02000a70();
s32 Func_02000a00();
s32 Actor_CommitPositionAndActivate();
s32 FieldEvent_Wait();
s32 Actor_GetById();
s32 Func_020009d0();
s32 Actor_SetPositionAndReset();
s32 Actor_ArmCallback();
s32 Motion_SetVarCbAndRefresh();
s32 Func_02000a08();
s32 Motion_SetModeAndWaitAnimation();
s32 Actor_SetModeById();
s32 Party_SetFields1eeAnd1f0();
s32 Party_SetFields1f2And1f4();
s32 Func_02000a40();
s32 Func_02000990();
extern u8 MsgHaThatWas[];
extern u8 gMainRoutineWork[];

void Func_02000494(s32 p0) {
  s32 v_r0, v_r2, v_r3, v_r5, v_r6;

  Call1((void (*)())Func_02000988, p0);
  Call1((void (*)())Func_02000a70, 0);
  Call1((void (*)())Func_02000a00, (s32)MsgHaThatWas);
  v_r6 = (s32)gMainRoutineWork;
  v_r5 = v_r6 + 0x214;
  v_r0 = FIELD_AT(v_r5, s32, 0);
  Call4((void (*)())Actor_CommitPositionAndActivate, v_r0, 0, -4, 0x214);
  Call1((void (*)())FieldEvent_Wait, 30);
  v_r0 = FIELD_AT(v_r5, s32, 0);
  v_r0 = Actor_GetById(v_r0);
  v_r0 = v_r0 + 90;
  v_r2 = FIELD_AT(v_r0, u8, 0);
  v_r3 = 254 & v_r2;
  FIELD_AT(v_r0, u8, 0) = v_r3;
  v_r0 = FIELD_AT(v_r5, s32, 0);
  Call4((void (*)())Actor_CommitPositionAndActivate, v_r0, 0, 12, v_r3);
  Call1((void (*)())FieldEvent_Wait, 10);
  v_r0 = FIELD_AT(v_r5, s32, 0);
  Call3((void (*)())Actor_CommitPositionAndActivate, v_r0, 0, 16);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call3((void (*)())Func_020009d0, 13, 0x2680000, 0x3900000);
  Call3((void (*)())Actor_SetPositionAndReset, 13, 0x258, 0x398);
  Call3((void (*)())Func_020009d0, 15, 0x2680000, 0x3900000);
  Call3((void (*)())Actor_ArmCallback, 13, 0x4000, 0);
  Call3((void (*)())Actor_SetPositionAndReset, 15, 0x278, 0x398);
  Call3((void (*)())Actor_ArmCallback, 15, 0x4000, 0);
  Call3((void (*)())Func_020009d0, 14, 0x2680000, 0x3900000);
  Call3((void (*)())Actor_SetPositionAndReset, 14, 0x268, 0x398);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 13, 2);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Func_02000a08, 13, 0, 10);
  Call3((void (*)())Func_02000a08, 14, 0, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 15, 3);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call3((void (*)())Func_02000a08, 15, 0, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 13, 3);
  Call2((void (*)())Actor_SetModeById, 14, 3);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 15, 3);
  Call2((void (*)())Party_SetFields1eeAnd1f0, 7, 99);
  Call2((void (*)())Party_SetFields1f2And1f4, 4, 98);
  v_r6 = v_r6 + 0x24b;
  FIELD_AT(v_r6, u8, 0) = 2;
  Call2((void (*)())Func_02000a40, 9, 2);
  Func_02000990();
}
