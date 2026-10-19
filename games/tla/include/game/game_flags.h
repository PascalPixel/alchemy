#ifndef ALCHEMY_GAME_FLAGS_H
#define ALCHEMY_GAME_FLAGS_H

#include "edition.h"
#include "types.h"

// The byte of gGameFlagBytes that holds `flag`: bits 3 to 11 of its number,
// isolated by shifting up and back down, which is how the game computes it.
#define GAME_FLAG_BYTE(flag) (((u32)(flag) << 20) >> 23)

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
