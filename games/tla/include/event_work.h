#ifndef TLA_EVENT_WORK_H
#define TLA_EVENT_WORK_H

#include "../../common/include/field/event_work.h"

struct PendingEvent {
  u32 key;
  u16 flags;
  u16 condition;
  s32 action;
};

void EventRuntime_RunMessage(s32 actor, s32 message);
void EventRuntime_ResolveAllPendingActions(void);
void EventRuntime_ResolvePendingActions(s32 skip);
s32 EventRuntime_ExecutePackedAction(u32 action);
s32 EventRuntime_RunPendingEvent(struct PendingEvent *event, s32 mode,
                                 s32 target);
s32 EventRuntime_GetControlledOwner(void);
void EventRuntime_ShowMessageAndWait(s32 actor, s32 flags);
void EventRuntime_UpdateWaitMode(void);
void EventRuntime_Wait(s32 frames);
void EventRuntime_PrepareCurrentObject(void);
void EventRuntime_Begin(void);
void EventRuntime_End(void);
void EventRuntime_EmptyHook(void);
void EventRuntime_StartScreen(void);
void EventRuntime_FinishScreen(void);
void EventRuntime_WaitDelay(void);
void EventRuntime_SetDestination(s32 scene, s32 entrance);
void EventRuntime_SetSavedDestination(s32 scene, s32 entrance);
void EventRuntime_SetCountdown(u16 countdown);
void EventRuntime_SetMessage(s32 message);

#endif
