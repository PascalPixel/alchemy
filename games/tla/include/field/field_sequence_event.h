#ifndef ALCHEMY_FIELD_SEQUENCE_EVENT_H
#define ALCHEMY_FIELD_SEQUENCE_EVENT_H

/// @file
/// The field sequence's event routines (field_sequence/event.c and items.c).

#include "field/field_work.h"

struct PendingEvent {
  u32 key;
  u16 flags;
  u16 condition;
  s32 action;
};

void Event_RunMessage(s32 actor, s32 message);
void Event_ResolveAllPendingActions(void);
void Event_ResolvePendingActions(s32 skip);
s32 Field_RunPsynergyEndEvents(u32 action);
s32 Field_RunPsynergyEvent(struct PendingEvent *event, s32 mode, s32 target);
s32 Party_GetLeaderActor(void);
void Event_ShowMessageAndWait(s32 actor, s32 flags);
void Event_UpdateDebugSkip(void);
void Event_Wait(s32 frames);
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
