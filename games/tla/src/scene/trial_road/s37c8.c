#include "trial_road.h"
#include "text/msg_ids.h"

void FieldEvent_WaitFrames(s32 frames);
void Task_Wait(s32 frames);
void Camera_MoveWithinMapBounds(s32 a, s32 b, s32 c, s32 d);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Actor_StartRepeatedMotion(s32 actor, s32 variant);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02005880(void);
void Func_020059e8(s32 value);
void Func_02002d78(s32 actor, s32 x, s32 z, s32 angle);
void Func_02005a48(s32 actor, s32 angle);
void Func_02005938(s32 message);
void Func_02005940(s32 actor, s32 flags);

static __inline__ void Camera_MoveWithinMapBounds_(s32 a, s32 b, s32 c, s32 d) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Camera_MoveWithinMapBounds(a, b, c, d);
}

void Func_020037c8(void) {
  Func_02005880();
  Func_020059e8(0);
  Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
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
  Func_02005a48(7, 0x4000);
  Actor_SetSingleChildFlags(Actor_GetById(4), 0);
  Actor_SetSingleChildFlags(Actor_GetById(5), 0);
  Actor_SetSingleChildFlags(Actor_GetById(6), 0);
  Actor_SetSingleChildFlags(Actor_GetById(7), 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_WaitFrames(30);
  Func_02005938((s32)MsgTrialRoadLeaveVillage);
  Motion_SetModeAndWaitAnimation(8, 3);
  Func_02005940(8, 0);
  Actor_StartRepeatedMotion(4, 2);
  Task_Wait(1);
  Actor_StartRepeatedMotion(6, 2);
  Task_Wait(2);
  Actor_StartRepeatedMotion(5, 2);
  Actor_StartRepeatedMotion(7, 2);
  FieldEvent_WaitFrames(15);
}
