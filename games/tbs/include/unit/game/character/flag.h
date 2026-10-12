#ifndef ALCHEMY_UNIT_GAME_CHARACTER_FLAG_H
#define ALCHEMY_UNIT_GAME_CHARACTER_FLAG_H

#include "types.h"

s32 GameFlag_Test(s32 flag);
s32 GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
u32 GameFlag_Toggle(s32 flag);
s32 GameFlag_GetByte(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
u8 GameFlag_IncrementByte(s32 flag);
u8 GameFlag_DecrementByte(s32 flag);
s32 GameFlag_GetNibble(u32 flag);
void GameFlag_SetNibble(s32 flag, s32 value);

#endif
