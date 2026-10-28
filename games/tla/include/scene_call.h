#ifndef TLA_SCENE_CALL_H
#define TLA_SCENE_CALL_H

#include "types.h"

// Scene forwarding. ⚓️'s scene files reach the main image through their own static
// inline wrappers, one per imported call, as ☀️'s scene layer (field_event.h)
// does. The wrapper's parameters keep each call's own constants and stack
// arguments in their own registers; called directly, nearby calls share a
// constant in one register or reuse r3 for each stack argument. Every wrapper
// carries the tag `FAKEMATCH: forwarded inline; see scene_call.h.`

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void FieldEvent_WaitFrames(s32 frames);
void Engine_EventStart(void);
void Engine_EventEnd(void);
void Engine_EventResolvePendingActions(s32 skip);
void Engine_EventSetMessage(s32 message);
void Engine_EventShowMessage(s32 speaker, s32 flags);
void Engine_ActorSetSpritePriority(s32 actor, s32 priority);
void ObjectMotion_Launch(s32 actor, s32 speed, s32 frames);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void FieldMessage_ShowAtActor(s32 speaker, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
void FieldMessage_Show(s32 message, s32 position);

#endif
