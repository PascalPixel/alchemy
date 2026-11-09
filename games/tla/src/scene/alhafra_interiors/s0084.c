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
  GameFlag_SetBit(0x8b2);
  Func_02000a94();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgAlhafraInteriors31);
  Func_02000b84(0x1f, 1);
  Actor_WalkToAndWait(4, 452, 335);
  Actor_FaceDirection(4, 0x8000, 0);
  Func_02000b7c();
  Actor_ArmCallback(31, 0x2000, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x801f, 0, 0xa);
  Actor_StartRepeatedMotion(4, 3);
  Func_080d489c(4, 0x102);
  Event_Wait(50);
  Motion_SetModeAndWaitAnimation(31, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x801f, 0, 0xa);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(31, 4);
  FieldEvent_ShowMessageThenWait(0x801f, 0, 0xa);
  Motion_SetVarCbAndRefresh(31, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x801f, 0, 0xa);
  Motion_SetModeAndWaitAnimation(31, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(0x801f, 0, 0xa);
  Actor_ArmCallback(31, 0xc000, 0);
  Event_Wait(20);
  Camera_FollowActor(FIELD_AT(gMainRoutineWork, s32, 532), 1);
  Func_02000b7c();
  Func_02000a9c();
}
