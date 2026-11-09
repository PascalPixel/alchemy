#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
s32 Func_02000a94();
void Func_02000a9c(void);
extern u8 gMainRoutineWork[];
extern u8 MsgAlhafraInteriors07[];

void Func_020002bc(void) {
  GameFlag_SetBit(0x8b1);
  Func_02000a94();
  Event_ResolvePendingActions(0);
  Actor_WalkToAndWait(FIELD_AT(gMainRoutineWork, s32, 532), 152, 328);
  Actor_FaceDirection(FIELD_AT(gMainRoutineWork, s32, 532), 0xc000, 0);
  Event_SetMessageId((s32)MsgAlhafraInteriors07);
  Actor_ArmCallback(14, 0, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0xe, 0, 0xa);
  Actor_FaceDirection(15, 0x8000, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(15, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0xf, 0, 0xa);
  Actor_SetSpeed(14, 0x8000, 0x4000);
  Actor_CommitPositionAndActivate(14, -32, 0);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0xe, 0, 0xa);
  Motion_SetVarCbAndRefresh(13, 2);
  Event_Wait(10);
  Actor_ArmCallback(13, 0, 0);
  Event_Wait(20);
  Func_080d47b4(0xd, 0x107, 0x1e);
  FieldEvent_ShowMessageThenWait(0xd, 0, 0xa);
  FieldActor_StartRepeatedMotion(14, 3);
  Actor_SetAttachedEffect(0xe, 0x102);
  FieldActor_StartRepeatedMotion(15, 3);
  Func_080d489c(0xf, 0x102);
  Event_Wait(50);
  Motion_SetVarCbAndRefresh(13, 2);
  Event_Wait(20);
  Actor_ArmCallback(13, 0xb000, 0);
  Event_Wait(10);
  Event_ShowMessageThenWait(0xd, 0, 0xa);
  Event_Wait(10);
  Actor_SetPositionAndReset(14, 152, 312);
  Event_Wait(10);
  Actor_SetAnimation(15, 4);
  Motion_SetModeAndWaitAnimation(14, 4);
  Event_Wait(10);
  Actor_FaceDirection(14, 0x3000, 0);
  Actor_FaceDirection(15, 0x4000, 0);
  Func_02000a9c();
}
