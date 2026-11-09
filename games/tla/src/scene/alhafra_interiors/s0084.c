#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
void Func_02000a94(void);
void Func_02000b84(s32 a, s32 b);
void Func_02000b7c(void);
void Func_02000a9c(void);
extern u8 MsgAlhafraInteriors31[];
extern u8 gMainRoutineWork[];

void Func_02000084(void) {
  Event_SetFlag(0x8b2);
  Func_02000a94();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgAlhafraInteriors31);
  Func_02000b84(0x1f, 1);
  Actor_WalkToAndWait(4, 452, 335);
  Actor_FaceDirection(4, 0x8000, 0);
  Func_02000b7c();
  Actor_FaceDirection(31, 0x2000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x801f, 0, 0xa);
  FieldActor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, 0x102);
  FieldEvent_Wait(50);
  Actor_SetAnimationAndWait(31, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x801f, 0, 0xa);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(31, 4);
  FieldEvent_ShowMessageThenWait(0x801f, 0, 0xa);
  Actor_RunRepeatedMotion(31, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x801f, 0, 0xa);
  Actor_SetAnimationAndWait(31, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x801f, 0, 0xa);
  Actor_FaceDirection(31, 0xc000, 0);
  FieldEvent_Wait(20);
  FieldCamera_FollowActor(FIELD_AT(gMainRoutineWork, s32, 532), 1);
  Func_02000b7c();
  Func_02000a9c();
}
