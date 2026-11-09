#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
s32 Func_02001be4();
void Func_02001ccc(void);
s32 Func_02001cec();
s32 Func_02001bec();
extern u8 MsgAlhafra74[];
extern u8 gMainRoutineWork[];

void Func_0200047c(void) {
  GameFlag_SetBit(0x8b7);
  Func_02001be4();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgAlhafra74);
  Camera_MoveWithinMapBounds(0x2300000, -1, 0x1f00000, 1);
  Func_02001ccc();
  Event_Wait(10);
  Motion_SetVarCbAndRefresh(29, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0xa);
  Actor_FaceDirection(28, 0, 0);
  Actor_FaceDirection(31, 0x6000, 0);
  Actor_FaceDirection(30, 0x6000, 0);
  Actor_SetModeById(29, 5);
  Sound_Play(0x59);
  FIELD_AT(gMainRoutineWork, s32, 532) = 0x1d;
  Func_02001cec(0, 3);
  FIELD_AT(gMainRoutineWork, s32, 532) = 4;
  Event_Wait(50);
  FieldActor_Jump(28, 6, 15);
  FieldActor_Jump(28, 6, 23);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 0xa);
  Actor_ArmCallback(29, 0x8000, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(29, 3);
  Event_Wait(20);
  Func_080d47b4(0x1e, 0x108, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1e, 0, 0xa);
  Func_080d47b4(0x1d, 0x102, 0x28);
  Actor_FaceDirection(29, 0x4000, 0);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(29, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0xa);
  Motion_SetVarCbAndRefresh(30, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1e, 0, 0xa);
  Actor_ArmCallback(29, 0, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(29, 3);
  Event_Wait(20);
  FieldActor_Jump(31, 6, 15);
  FieldActor_Jump(31, 6, 23);
  FieldEvent_ShowMessageThenWait(0x1f, 0, 0xa);
  Actor_FaceDirection(30, 0, 0);
  Motion_SetModeAndWaitAnimation(30, 3);
  Event_Wait(20);
  Actor_FaceDirection(31, 0x4000, 0);
  Actor_SetAnimation(31, 6);
  Actor_FaceDirection(30, 0xc000, 0);
  Actor_SetAnimation(30, 6);
  Actor_FaceDirection(29, 0x4000, 0);
  Actor_FaceDirection(28, 0xc000, 0);
  FieldCamera_FollowActor(FIELD_AT(gMainRoutineWork, s32, 532), 1);
  Func_02001ccc();
  Func_02001bec();
}
