#include "trial_road.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02005880(void);
void Func_02002d78(s32 actor, s32 x, s32 z, s32 angle);

void Func_020037c8(void) {
  Func_02005880();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_02002d78(8, 264, 264, 0x8000);
  Func_02002d78(13, 280, 248, 0x8000);
  Func_02002d78(14, 280, 280, 0x8000);
  Func_02002d78(4, 240, 276, 0);
  Func_02002d78(7, 216, 254, 0);
  Func_02002d78(5, 232, 244, 0);
  Func_02002d78(6, 224, 296, 0);
  Actor_SetModeById(4, 38);
  Actor_SetModeById(5, 19);
  Actor_SetModeById(6, 19);
  Actor_SetModeById(7, 19);
  Func_080d3928(7, 0x4000);
  Actor_SetSingleChildFlags(Actor_GetById(4), 0);
  Actor_SetSingleChildFlags(Actor_GetById(5), 0);
  Actor_SetSingleChildFlags(Actor_GetById(6), 0);
  Actor_SetSingleChildFlags(Actor_GetById(7), 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(30);
  Event_SetMessageId((s32)MsgTrialRoadLeaveVillage);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_ShowMessageAndWait(8, 0);
  Actor_StartRepeatedMotion(4, 2);
  wait(1);
  Actor_StartRepeatedMotion(6, 2);
  wait(2);
  Actor_StartRepeatedMotion(5, 2);
  Actor_StartRepeatedMotion(7, 2);
  Event_Wait(15);
}
