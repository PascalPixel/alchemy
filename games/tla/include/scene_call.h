#ifndef TLA_SCENE_CALL_H
#define TLA_SCENE_CALL_H

/// @file
/// The routines the scenes' own event inlines call, declared once for the
/// files that have not yet moved to event_inline.h.
///
/// A scene file that wraps a routine in a small inline of its own does what
/// event_inline.h does: it is a habit of the whole game written once, a layer
/// and not a compiler-steering device. The inline's parameters are its own, so
/// each call builds its constants and stack arguments again.

#include "types.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void FieldEvent_Wait(s32 frames);
void Import_EventStart(void);
void Import_EventEnd(void);
void Import_EventResolvePendingActions(s32 skip);
void Import_EventSetMessageId(s32 message);
void Import_EventShowMessageAndWait(s32 speaker, s32 flags);
void Import_ActorSetSpritePriority(s32 actor, s32 priority);
void Actor_Jump(s32 actor, s32 speed, s32 frames);
void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Actor_SetAngleToward(s32 actor, s32 target, s32 frames);
void FieldMessage_ShowAtActor(s32 speaker, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
void FieldMessage_Show(s32 message, s32 position);

#endif
