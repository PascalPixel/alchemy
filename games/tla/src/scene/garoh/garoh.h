#ifndef TLA_GAROH_H
#define TLA_GAROH_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "flag_call.h"

struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void FieldEvent_WaitFrames(s32 frames);
void FieldMessage_ShowAtActor(s32 id, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
void Task_Add(void (*callback)(void), s32 priority);
void Event_WaitTransitionFrames(void);

void Func_02005184(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void Func_0200518c(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void Func_020051ac(void);
void Func_020051b4(void);
void Func_0200524c(s32 message);
void Func_0200525c(s32 id, s32 a, s32 b);
void Func_02005264(s32 id, s32 a);
void Func_020052cc(s32 a, s32 b);
void Func_020052d4(s32 a);
void Func_02005324(s32 a);
void Func_02005354(s32 a, s32 b, s32 c);
void Func_0200536c(const void *data);
void Func_02005374(const void *data);
void Func_0200537c(void);
s32 Func_02005384(void);
void Func_02005394(s32 cue);

void Func_02000e78(s32 id, struct Actor *object);
void Func_02000eb0(void);
void Func_02000eec(void);
void Func_02000f28(void);
void Func_02000f3c(void);
void Func_020012a8(s32 id);

#endif
