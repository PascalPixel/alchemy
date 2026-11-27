#ifndef ALCHEMY_BATTLE_SIDE_H
#define ALCHEMY_BATTLE_SIDE_H

// Which units are in a battle, and the Djinn recovery list.

#include "edition.h"
#include "static_assert.h"

// UNIT.side: which side a unit fights on, if it is in the battle.
typedef enum BATTLE_SIDE
{
    BATTLE_SIDE_NONE  = 0,
    BATTLE_SIDE_ENEMY = 1,
    BATTLE_SIDE_PARTY = 2
} BATTLE_SIDE;

// Marks in a list of unit ids, such as BATTLE_STATE.partyUnits and enemyIds.
typedef enum BATTLE_UNIT_LIST_MARK
{
    BATTLE_UNIT_REMOVED  = 254, // a place whose unit has left the battle
    BATTLE_UNIT_LIST_END = 255  // ends the list
} BATTLE_UNIT_LIST_MARK;

// The list place codes the target pickers return (battle_targeting.c, mnBattleTargetRunSelection,
// btFindUnitPlace): a place in the party's list or the enemies', or'd with the side's tag.
#define PLACE_PARTY      (1 << 8)
#define PLACE_ENEMY      (PLACE_PARTY | PLACE_ENEMY_SIDE)
// A place code's side bit, set for the enemies' list, and its place in the list (btGetUnitAtPlace).
#define PLACE_ENEMY_SIDE (1 << 7)
#define PLACE_INDEX_MASK ((1 << 4) - 1)

// Which sides a unit list takes, as btUnitListLiving reads it.
typedef enum BATTLE_SIDE_MASK
{
    BATTLE_SIDE_MASK_PARTY   = 1,
    BATTLE_SIDE_MASK_ENEMIES = 2,
    BATTLE_SIDE_MASK_BOTH    = BATTLE_SIDE_MASK_PARTY | BATTLE_SIDE_MASK_ENEMIES
} BATTLE_SIDE_MASK;

// The order the two sides take their turn-end effects in.
typedef struct BATTLE_SIDE_ORDER
{
    Sint32 group[2]; // BATTLE_SIDE_MASK: the party, then the enemies
} BATTLE_SIDE_ORDER;

// The Djinn recovery list's two tables (gmDjinnGetRecoveryTableStub): the party's and the link
// opponent's.
#define DJINN_RECOVERY_PARTY        0
#define DJINN_RECOVERY_OPPONENT     1

// The enemy unit whose record holds the link opponent's Djinn recovery table: the opponent's three
// party members take the three before it (scBattleArenaReceivePartyRecords), and the table fits in
// one unit's record.
#define OPPONENT_RECOVERY_UNIT      (FIRST_ENEMY_UNIT + 3)

// DJINN_RECOVERY_ENTRY.turns of a Djinni on standby. In battle it stays so until a Djinn command
// sets it again or a summon uses it (DJINN_RECOVERY_SUMMONED), both in btActionRunBuild; a party
// member who does not fight has it set as the next battle starts, outside a link battle
// (btDjinnRecoveryReconcile). btDjinnRecoveryCountDown counts down only positive turns.
#define DJINN_RECOVERY_AFTER_BATTLE -1

// DJINN_RECOVERY_ENTRY.turns of a standby Djinni a summon has just used, until the summoner's
// action ends and sets its count of turns to recover (btUnitProcessActionEnd).
#define DJINN_RECOVERY_SUMMONED     -2

// A Djinni on standby or recovering: its element and index, the unit it belongs to, and the turns
// until it is set again.
typedef struct DJINN_RECOVERY_ENTRY
{
    Uint8 element; // ELEMENT
    Uint8 index;   // the Djinni within its element
    Uint8 unitId;  // the unit it belongs to
    // Turns until it is set again, or DJINN_RECOVERY_AFTER_BATTLE or DJINN_RECOVERY_SUMMONED.
    Sint8 turns;
} DJINN_RECOVERY_ENTRY;

STATIC_ASSERT(sizeof(DJINN_RECOVERY_ENTRY) == 4);

// One side's Djinn recovery list: up to 64 entries and their count.
typedef struct DJINN_RECOVERY_LIST
{
    DJINN_RECOVERY_ENTRY entries[64];
    Sint32               count; // entries in use
} DJINN_RECOVERY_LIST;

// One side's Djinn recovery table, as gmDjinnGetRecoveryTable returns it: the party's
// MainRoutineStateWork.holdings, the enemies' over battle unit OPPONENT_RECOVERY_UNIT.
typedef struct DJINN_RECOVERY_TABLE
{
    // The summons the party can call, bit n for summon n; nonzero offers the Summon command
    // (btSummonUpdateAvailability).
    Uint32              summonMask;
    // In the party's table, MainRoutineStateWork.holdings.coins; not named here.
    Uint32              partyCoins;
    DJINN_RECOVERY_LIST list;
} DJINN_RECOVERY_TABLE;

#endif
