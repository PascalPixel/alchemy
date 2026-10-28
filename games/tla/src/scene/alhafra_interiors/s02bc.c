#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 GameFlag_SetBit();
s32 Func_02000a94();
s32 Func_02000b8c();
s32 Actor_SetPositionAndReset();
s32 Actor_ArmCallback();
s32 Func_02000b24();
s32 FieldEvent_Wait();
s32 Func_02000b34();
s32 Motion_SetModeAndWaitAnimation();
s32 Actor_SetSpeedParameters();
s32 Actor_CommitPositionAndActivate();
s32 Motion_SetVarCbAndRefresh();
s32 Func_02000b5c();
s32 Actor_StartRepeatedMotion();
s32 Func_02000b64();
s32 Actor_SetModeById();
void Func_02000a9c(void);
extern u8 gMainRoutineWork[];
extern u8 MsgAlhafraInteriors07[];

void Func_020002bc(void) {
  Call1(GameFlag_SetBit, 0x8b1);
  Func_02000a94();
  Call1(Func_02000b8c, 0);
  Call3(Actor_SetPositionAndReset, FIELD_AT(gMainRoutineWork, s32, 532), 152,
        328);
  Call3(Actor_ArmCallback, FIELD_AT(gMainRoutineWork, s32, 532), 0xc000, 0);
  Call1(Func_02000b24, (s32)MsgAlhafraInteriors07);
  Call3(Actor_ArmCallback, 14, 0, 0);
  Call1(FieldEvent_Wait, 20);
  Call3(Func_02000b34, 0xe, 0, 0xa);
  Call3(Actor_ArmCallback, 15, 0x8000, 0);
  Call1(FieldEvent_Wait, 20);
  Call2(Motion_SetModeAndWaitAnimation, 15, 3);
  Call1(FieldEvent_Wait, 10);
  Call3(Func_02000b34, 0xf, 0, 0xa);
  Call3(Actor_SetSpeedParameters, 14, 0x8000, 0x4000);
  Call3(Actor_CommitPositionAndActivate, 14, -32, 0);
  Call1(FieldEvent_Wait, 10);
  Call3(Func_02000b34, 0xe, 0, 0xa);
  Call2(Motion_SetVarCbAndRefresh, 13, 2);
  Call1(FieldEvent_Wait, 10);
  Call3(Actor_ArmCallback, 13, 0, 0);
  Call1(FieldEvent_Wait, 20);
  Func_02000b5c(0xd, 0x107, 0x1e);
  Call3(Func_02000b34, 0xd, 0, 0xa);
  Call2(Actor_StartRepeatedMotion, 14, 3);
  Call2(Func_02000b64, 0xe, 0x102);
  Call2(Actor_StartRepeatedMotion, 15, 3);
  Call2(Func_02000b64, 0xf, 0x102);
  Call1(FieldEvent_Wait, 50);
  Call2(Motion_SetVarCbAndRefresh, 13, 2);
  Call1(FieldEvent_Wait, 20);
  Call3(Actor_ArmCallback, 13, 0xb000, 0);
  Call1(FieldEvent_Wait, 10);
  Call3(Func_02000b34, 0xd, 0, 0xa);
  Call1(FieldEvent_Wait, 10);
  Call3(Actor_SetPositionAndReset, 14, 152, 312);
  Call1(FieldEvent_Wait, 10);
  Call2(Actor_SetModeById, 15, 4);
  Call2(Motion_SetModeAndWaitAnimation, 14, 4);
  Call1(FieldEvent_Wait, 10);
  Call3(Actor_ArmCallback, 14, 0x3000, 0);
  Call3(Actor_ArmCallback, 15, 0x4000, 0);
  Func_02000a9c();
}
