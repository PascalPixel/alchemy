#ifndef ALCHEMY_UNIT_BATTLE_SMNENTRY_H
#define ALCHEMY_UNIT_BATTLE_SMNENTRY_H

#include "types.h"

u32 Summon_GetLayoutKind(s32 index);
s32 Summon_GetEntryValue(s32 index);
s32 Summon_GetEntryFlag1Field(s32 index);
s32 Summon_IsEntryFlagged(s32 index);
u32 Battle_GetSpriteFlagNibble(u32 index);
u32 Battle_GetSpriteFlagHighBits(u32 index);
s32 Summon_IsEntrySecondaryFlagged(s32 index);
s32 Summon_GetEntryByte4(s32 index);

#endif
