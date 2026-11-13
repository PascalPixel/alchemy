#ifndef TLA_GAROH_H
#define TLA_GAROH_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "flag_call.h"

void Event_WaitTransitionFrames(void);

void Event_Start(void);
void Event_End(void);
void Func_02005354(s32 a, s32 b, s32 c);
void Func_0200536c(const void *data);
void Func_02005374(const void *data);
void Func_0200537c(void);

void Func_02000e78(s32 id, struct Actor *object);
void Func_02000eb0(void);
void Func_02000eec(void);
void Func_02000f28(void);
void Func_02000f3c(void);
void Func_020012a8(s32 id);

#endif
