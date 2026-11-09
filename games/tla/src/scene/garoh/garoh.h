#ifndef TLA_GAROH_H
#define TLA_GAROH_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "flag_call.h"

struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Event_Wait(s32 frames);
void FieldMessage_ShowAtActor(s32 id, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
void Task_Add(void (*func)(void), s32 priority);
void Event_WaitTransitionFrames(void);

void Map_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void Map_CopyCellAttributeRect(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void Func_020051ac(void);
void Func_020051b4(void);
void Event_SetMessageId(s32 message);
void Event_ShowMessageThenWait(s32 id, s32 a, s32 b);
void Event_ShowMessageAndWait(s32 id, s32 a);
void BattleEffect_ApplyColorToTargetBuffer(s32 a, s32 b);
void BattleEffect_StartBufferInterpolation(s32 a);
void Event_ResolvePendingActions(s32 a);
void Func_02005354(s32 a, s32 b, s32 c);
void Func_0200536c(const void *data);
void Func_02005374(const void *data);
void Func_0200537c(void);
s32 Party_GetLeaderActor(void);
void Sound_Play(s32 cue);

void Func_02000e78(s32 id, struct Actor *object);
void Func_02000eb0(void);
void Func_02000eec(void);
void Func_02000f28(void);
void Func_02000f3c(void);
void Func_020012a8(s32 id);

#endif
