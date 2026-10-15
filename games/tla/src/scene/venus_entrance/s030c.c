#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 GameFlag_SetBit();
s32 Object_GetById();
s32 Object_SetPositionAndResetMotion();
s32 Func_0200297c();
s32 Motion_CamBounds();
s32 Event_StartScreenTransition();
s32 Event_WaitTransitionFrames();
s32 Object_SetModeById();
s32 Engine_AudioPlayCue();
s32 Func_02002884();
s32 Func_02002864();
s32 Battle_WaitMode0();
s32 ObjectMotion_CommitCurrentPositionAndActivate();
s32 Func_0200298c();
extern u8 gData_0200b000[];

void Func_0200030c(void) {
  s32 t1;
  s32 t3;
  s32 t9;
  s32 t16;

  Call1(GameFlag_SetBit, 0x891);
  t1 = Value1(Object_GetById, 0xa);
  FIELD_AT((t1 + 0x55), u8, 0) = 0;
  FIELD_AT(t1, s32, 72) = 0x28f;
  FIELD_AT(t1, s32, 48) = 0x18000;
  FIELD_AT(t1, s32, 52) = 0x8000;
  Call4(Object_SetPositionAndResetMotion, t1, 0xd80000, 0x600000, 0xb00000);
  t3 = Value1(Object_GetById, 0xb);
  FIELD_AT((t3 + 0x55), u8, 0) = 0;
  FIELD_AT(t3, s32, 72) = 0x7ae;
  FIELD_AT(t3, s32, 48) = 0x18000;
  FIELD_AT(t3, s32, 52) = 0x8000;
  Call4(Object_SetPositionAndResetMotion, t3, 0x2680000, 0x600000, 0x980000);
  Call2(Func_0200297c, 0xcccc, 0x1999);
  Call4(Motion_CamBounds, 0x1780000, -1, 0xf80000, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  t9 = Value1(Object_GetById, 0xa);
  Call2(Object_SetModeById, 0xa, 4);
  Call1(Engine_AudioPlayCue, 0xa5);
  Call4(Func_02002884, t9, 0x1600000, 0x80000, 0xb00000);
  Call2(Func_02002864, t9, (s32)gData_0200b000);
  Call1(Battle_WaitMode0, 0x19);
  Call4(Motion_CamBounds, 0x1780000, -1, 0x680000, 1);
  t16 = Value1(Object_GetById, 0xb);
  Call2(Object_SetModeById, 0xb, 4);
  Call4(Func_02002884, t16, 0x1a00000, 0x80000, 0x980000);
  Call2(Func_02002864, t16, (s32)gData_0200b000);
  Call1(Battle_WaitMode0, 0x3c);
  Call1(Engine_AudioPlayCue, 0xa5);
  Call1(ObjectMotion_CommitCurrentPositionAndActivate, 0xa);
  Call1(ObjectMotion_CommitCurrentPositionAndActivate, 0xb);
  Func_0200298c();
}
