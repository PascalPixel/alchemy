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
  Event_SetFlag(0x8b1);
  Func_02000a94();
  FieldEvent_ResolvePendingActions(0);
  Actor_WalkToAndWait(FIELD_AT(gMainRoutineWork, s32, 532), 152, 328);
  Actor_FaceDirection(FIELD_AT(gMainRoutineWork, s32, 532), 0xc000, 0);
  FieldEvent_SetMessageId((s32)MsgAlhafraInteriors07);
  Actor_FaceDirection(14, 0, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0xe, 0, 0xa);
  Actor_FaceDirection(15, 0x8000, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(15, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0xf, 0, 0xa);
  Actor_SetSpeed(14, 0x8000, 0x4000);
  Actor_WalkByAndWait(14, -32, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0xe, 0, 0xa);
  Actor_RunRepeatedMotion(13, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(13, 0, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(0xd, 0x107, 0x1e);
  FieldEvent_ShowMessageThenWait(0xd, 0, 0xa);
  FieldActor_StartRepeatedMotion(14, 3);
  Actor_SetAttachedEffect(0xe, 0x102);
  FieldActor_StartRepeatedMotion(15, 3);
  Actor_SetAttachedEffect(0xf, 0x102);
  FieldEvent_Wait(50);
  Actor_RunRepeatedMotion(13, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(13, 0xb000, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0xd, 0, 0xa);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(14, 152, 312);
  FieldEvent_Wait(10);
  Actor_SetAnimation(15, 4);
  Actor_SetAnimationAndWait(14, 4);
  FieldEvent_Wait(10);
  Actor_FaceDirection(14, 0x3000, 0);
  Actor_FaceDirection(15, 0x4000, 0);
  Func_02000a9c();
}
