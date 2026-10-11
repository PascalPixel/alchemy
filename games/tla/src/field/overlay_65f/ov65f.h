#ifndef TLA_OVERLAY_65F_OV65F_H
#define TLA_OVERLAY_65F_OV65F_H

#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "flag_call.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Battle_WaitMode0(s32 frames);
void UiText_OpenMessageAtObject(s32 id, s32 flags);
s32 Inventory_PromptAndSetObjectMode(s32 id, s32 flags);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Event_WaitTransitionFrames(void);

void Func_02005184(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Func_0200518c(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
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

void Func_02000e78(s32 id, struct ObjectRuntime *object);
void Func_02000eb0(void);
void Func_02000eec(void);
void Func_02000f28(void);
void Func_02000f3c(void);
void Func_020012a8(s32 id);

#endif
