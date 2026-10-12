#ifndef TLA_OVERLAY_6A4_OV6A4_H
#define TLA_OVERLAY_6A4_OV6A4_H

#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern u8 gDecodeFillByte;

void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndCommit(s32 actor, s32 x, s32 z);
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
struct ObjectRuntime *Func_02002af8(void);
void Func_02002b68(s32 cue);
void Func_02002a40(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02001010(struct ObjectRuntime *object);
void Func_020023b0(void);

#endif
