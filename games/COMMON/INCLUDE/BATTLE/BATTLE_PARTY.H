#ifndef ALCHEMY_BATTLE_PARTY_H
#define ALCHEMY_BATTLE_PARTY_H

#include "LAYOUT_GUARD.H"

enum BattleUnitPresence {
    BATTLE_UNIT_ABSENT = 0,
    BATTLE_UNIT_ENEMY = 1,
    BATTLE_UNIT_PARTY = 2
};

enum BattleUnitListMark {
    BATTLE_UNIT_REMOVED = 0xfe,
    BATTLE_UNIT_LIST_END = 0xff
};

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

LAYOUT_SIZE_GUARD(DjinnRecoveryEntry_Size, struct DjinnRecoveryEntry, 4);

struct DjinnRecoveryList {
    struct DjinnRecoveryEntry entries[64];
    s32 count;
};

struct DjinnRecoveryTable {
    u8 unknown_00[8];
    struct DjinnRecoveryList list;
};


s32 BattleParty_PrepareActiveOwners(u16 *unit_ids);
s32 BattleParty_ListPresentEnemies(s16 *unit_ids);
s32 BattleParty_ListLivingUnits(s32 side_mask, u16 *unit_ids);
s32 BattleParty_AssignMemberSlots(void);
s32 BattleParty_ListActorIds(s32 side_mask, u16 *unit_ids);

/* Countdown bytes share the live BattleUnit record. The modifier bytes are
   signed; confusion and reflect are read as unsigned countdowns. */
s32 BattleUnit_RollCounterEarlyEnd(s32 unit_id, s32 turns, s32 bias);
s32 BattleUnit_TickCounter132(s32 unit_id);
s32 BattleUnit_TickCounter134(s32 unit_id);
s32 BattleUnit_TickCounter136(s32 unit_id);
s32 BattleUnit_TickCounter138(s32 unit_id);
s32 BattleUnit_TickCounter139(s32 unit_id);
s32 BattleUnit_TickCounter13a(s32 unit_id);
s32 BattleUnit_TickCounter13b(s32 unit_id);
s32 BattleUnit_TickCounter13c(s32 unit_id);
s32 Battle_AdvanceCounterAndCheckChance(s32 unit_id);
s32 BattleUnit_TickCounter13f(s32 unit_id);
s32 BattleUnit_TickCounter146(s32 unit_id);
s32 BattlePlacement_UpdateTimedEntries(void);
s32 BattlePlacement_UpdateTimedEntriesTwentyTimes(void);

#endif
