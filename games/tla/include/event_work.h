#ifndef TLA_EVENT_WORK_H
#define TLA_EVENT_WORK_H

#include "field/event_work.h"

struct PendingEvent {
  u32 key;
  u16 flags;
  u16 condition;
  s32 action;
};

void Event_RunMessage(s32 actor, s32 message);
void Event_ResolveAllPendingActions(void);
void Event_ResolvePendingActions(s32 skip);
s32 Event_ExecutePackedAction(u32 action);
s32 Field_RunPsynergyEvent(struct PendingEvent *event, s32 mode,
                                 s32 target);
s32 Party_GetLeaderActor(void);
void Event_ShowMessageAndWait(s32 actor, s32 flags);
void Event_UpdateDebugSkip(void);
void Event_WaitFrames(s32 frames);
void Leader_StopMoving(void);
void Event_Start(void);
void Event_End(void);
void Event_EmptyCallback(void);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_SetDestination(s32 scene, s32 entrance);
void Event_SetRestartPoint(s32 scene, s32 entrance);
void Event_SetExitRequest(u16 countdown);
void Event_SetMessageId(s32 message);

#endif
