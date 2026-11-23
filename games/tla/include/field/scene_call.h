#ifndef ALCHEMY_SCENE_CALL_H
#define ALCHEMY_SCENE_CALL_H

/// @file
/// The event routines that some scenes wrap in small inlines of their own,
/// where the rest call event_inline.h's.

#include "types.h"

s32 GameFlag_Get(s32 flag);
void GameFlag_Set(s32 flag);
void GameFlag_Clear(s32 flag);
void FieldEvent_Wait(s32 frames);
void Actor_Jump(s32 actor, s32 speed, s32 frames);
void Actor_WalkToAndWaitById(s32 actor, s32 x, s32 z);
void Actor_FaceDirectionById(s32 actor, s32 angle, s32 frames);
void Actor_FaceActorById(s32 actor, s32 target, s32 frames);
s32 FieldMessage_ShowAtActor(s32 speaker, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
void FieldMessage_Show(s32 message, s32 position);

#endif
