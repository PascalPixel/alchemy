#ifndef ALCHEMY_GAME_FLAGS_H
#define ALCHEMY_GAME_FLAGS_H

#include "types.h"

/* Raw linkage names retained for the exact owner registry. */

s32 GameFlag_Test();
s32 GameFlag_Set();
void GameFlag_Clear(s32 flag);
u32 GameFlag_Toggle(s32 flag);
u8 GameFlag_GetByte(s32 flag);
u32 GameFlag_SetByte(u32 flag, u8 value);
u8 GameFlag_IncrementByte(s32 flag);
u8 GameFlag_DecrementByte(s32 flag);
s32 GameFlag_GetNibble(u32 flag);
void GameFlag_SetNibble(s32 flag, s32 value);

#endif
