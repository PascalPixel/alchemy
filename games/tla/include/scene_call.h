#ifndef TLA_SCENE_CALL_H
#define TLA_SCENE_CALL_H

#include "types.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void FieldEvent_WaitFrames(s32 frames);
void Engine_EventBegin(void);
void Engine_EventEnd(void);
void Engine_EventResolvePendingActions(s32 skip);
void Engine_EventSetMessage(s32 message);
void Engine_EventShowMessage(s32 speaker, s32 flags);
void Engine_ActorSetSpritePriority(s32 actor, s32 priority);
void ObjectMotion_Launch(s32 actor, s32 speed, s32 frames);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void UiText_OpenMessageAtObject(s32 speaker, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);

#endif
