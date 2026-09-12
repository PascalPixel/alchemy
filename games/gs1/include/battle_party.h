#ifndef ALCHEMY_BATTLE_PARTY_H
#define ALCHEMY_BATTLE_PARTY_H

#include "types.h"

enum BattleSideMask {
    BATTLE_SIDE_PARTY = 1,
    BATTLE_SIDE_ENEMIES = 2,
    BATTLE_SIDE_BOTH = BATTLE_SIDE_PARTY | BATTLE_SIDE_ENEMIES
};

#define BattleParty_ListActiveMembers Func_080b6a60
s32 BattleParty_ListActiveMembers(u16 *unit_ids);
s32 BattleParty_ListPresentEnemies(s16 *unit_ids);
s32 BattleParty_ListLivingUnits(s32 side_mask, u16 *unit_ids);
s32 BattleParty_AssignMemberSlots(void);

#endif
