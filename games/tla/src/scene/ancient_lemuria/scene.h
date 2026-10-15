#ifndef TLA_ANCIENT_LEMURIA_SCENE_H
#define TLA_ANCIENT_LEMURIA_SCENE_H

#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "field_event.h"

#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

struct FieldActor *Object_GetById(s32 id);
s32 Math_Sine(s32 angle);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_02000080(s32 id);
void Func_02000838(void);
void Func_020019f4(void);
void Func_02001a04(s32 a, s32 b, s32 c);
void Func_02001a24(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX, s32 destY, s32 layer);
void Func_02001a2c(s32 cue);

static inline void MapCopy(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX, s32 destY, s32 layer)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep each stack argument in its own register */
    Func_02001a24(srcX, srcY, width, height, destX, destY, layer);
}

#endif
