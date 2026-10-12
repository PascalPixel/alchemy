#ifndef ALCHEMY_UNIT_GAME_CHARACTER_CHARA4_H
#define ALCHEMY_UNIT_GAME_CHARACTER_CHARA4_H

#include "types.h"

s32 BtlUnitSet(s32 unit_id, s32 enemy_id, s32 suffix);
s32 Party_CountActiveOwners(void);
s32 Party_AddActiveOwner(s32 value);
s32 Party_RemoveActiveOwner(s32 value);
s32 Party_ListActiveOwners(s16 *owners);
s32 Party_AdjustCoins(s32 amount);

#endif
