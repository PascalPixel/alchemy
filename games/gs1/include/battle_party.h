#ifndef ALCHEMY_BATTLE_PARTY_H
#define ALCHEMY_BATTLE_PARTY_H

#include "types.h"

enum BattleSideMask {
    BATTLE_SIDE_PARTY = 1,
    BATTLE_SIDE_ENEMIES = 2,
    BATTLE_SIDE_BOTH = BATTLE_SIDE_PARTY | BATTLE_SIDE_ENEMIES
};

s32 Func_080b6a60(u16 *unit_ids);
s32 Func_080b6ae0(s16 *unit_ids);
s32 Func_080b6b40(s32 side_mask, u16 *unit_ids);
s32 Func_080b6378(void);

#define BattleParty_ListActiveMembers Func_080b6a60
#define BattleParty_ListPresentEnemies Func_080b6ae0
#define BattleParty_ListLivingUnits Func_080b6b40
#define BattleParty_AssignMemberSlots Func_080b6378

#endif
