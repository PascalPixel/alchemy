#ifndef ALCHEMY_BATTLE_PARTY_H
#define ALCHEMY_BATTLE_PARTY_H

#include "TYPES.H"

enum BattleSideMask {
    BATTLE_SIDE_PARTY = 1,
    BATTLE_SIDE_ENEMIES = 2,
    BATTLE_SIDE_BOTH = BATTLE_SIDE_PARTY | BATTLE_SIDE_ENEMIES
};

/* A side's set-aside Djinn: element, index within the element, owner and
   the turns left before it is set again. Summoning marks a Djinn -2 until
   the turn ends and it joins the recovery order. */
struct DjinnRecoveryEntry {
    u8 element;
    u8 index;
    u8 unit_id;
    s8 turns;
};

struct DjinnRecoveryList {
    struct DjinnRecoveryEntry entries[64];
    s32 count;
};

struct DjinnRecoveryTable {
    u8 unknown_00[8];
    struct DjinnRecoveryList list;
};


#define BattleParty_ListActiveMembers Func_080b6a60
s32 BattleParty_ListActiveMembers(u16 *unit_ids);
s32 BattleParty_ListPresentEnemies(s16 *unit_ids);
s32 BattleParty_ListLivingUnits(s32 side_mask, u16 *unit_ids);
s32 BattleParty_AssignMemberSlots(void);

#endif
