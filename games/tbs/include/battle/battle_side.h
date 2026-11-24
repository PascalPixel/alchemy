#ifndef ALCHEMY_BATTLE_SIDE_H
#define ALCHEMY_BATTLE_SIDE_H

/// @file
/// Which units are in a battle, and the Djinn recovery list.

#include "edition.h"
#include "static_assert.h"

/// Unit.side: which side a unit fights on, if it is in the battle.
enum BattleSide {
  BATTLE_SIDE_NONE = 0,
  BATTLE_SIDE_ENEMY = 1,
  BATTLE_SIDE_PARTY = 2
};

/// Marks in a list of unit ids, such as BattleState.partyUnits and enemyIds.
enum BattleUnitListMark {
  BATTLE_UNIT_REMOVED = 254, ///< a place whose unit has left the battle
  BATTLE_UNIT_LIST_END = 255 ///< ends the list
};

/// The list place codes the target pickers return (battle_targeting.c,
/// BattleTarget_RunSelection, Battle_FindUnitPlace): a place in the party's
/// list or the enemies', or'd with the side's tag.
#define PLACE_PARTY (1 << 8)
#define PLACE_ENEMY (PLACE_PARTY | PLACE_ENEMY_SIDE)
/// A place code's side bit, set for the enemies' list, and its place in the
/// list (Battle_GetUnitAtPlace).
#define PLACE_ENEMY_SIDE (1 << 7)
#define PLACE_INDEX_MASK ((1 << 4) - 1)

/// Which sides a unit list takes, as BattleUnit_ListLiving reads it.
enum BattleSideMask {
  BATTLE_SIDE_MASK_PARTY = 1,
  BATTLE_SIDE_MASK_ENEMIES = 2,
  BATTLE_SIDE_MASK_BOTH = BATTLE_SIDE_MASK_PARTY | BATTLE_SIDE_MASK_ENEMIES
};

/// The order the two sides take their turn-end effects in.
struct BattleSideOrder {
  s32 group[2]; ///< enum BattleSideMask: the party, then the enemies
};

/// The Djinn recovery list's two tables (Djinn_GetRecoveryTableFar): the
/// party's and the link opponent's.
#define DJINN_RECOVERY_PARTY 0
#define DJINN_RECOVERY_OPPONENT 1

/// DjinnRecoveryEntry.turns of a Djinni on standby. In battle it stays so
/// until a Djinn command sets it again or a summon uses it
/// (DJINN_RECOVERY_SUMMONED), both in
/// BattleActionRun_Build; a party member who does not fight has it set as the
/// next battle starts, outside a link battle
/// (DjinnRecovery_Reconcile). DjinnRecovery_CountDown counts down only
/// positive turns.
#define DJINN_RECOVERY_AFTER_BATTLE -1

/// DjinnRecoveryEntry.turns of a standby Djinni a summon has just used, until
/// the summoner's action ends and sets its count of turns to recover
/// (BattleUnit_ProcessActionEnd).
#define DJINN_RECOVERY_SUMMONED -2

/// A Djinni on standby or recovering: its element and index, the unit it
/// belongs to, and the turns until it is set again.
struct DjinnRecoveryEntry {
  u8 element; ///< enum Element
  u8 index;   ///< the Djinni within its element
  u8 unitId;  ///< the unit it belongs to
  /// Turns until it is set again, or DJINN_RECOVERY_AFTER_BATTLE or
  /// DJINN_RECOVERY_SUMMONED.
  s8 turns;
};

STATIC_ASSERT(sizeof(struct DjinnRecoveryEntry) == 4);

/// One side's Djinn recovery list: up to 64 entries and their count.
struct DjinnRecoveryList {
  struct DjinnRecoveryEntry entries[64];
  s32 count; ///< entries in use
};

/// One side's Djinn recovery table, as Djinn_GetRecoveryTable returns it: the
/// party's gMainRoutineState.holdings, the enemies' over
/// battle unit FIRST_ENEMY_UNIT + 3.
struct DjinnRecoveryTable {
  /// The summons the party can call, bit n for summon n; nonzero offers the
  /// Summon command (BattleSummon_UpdateAvailability).
  u32 summonMask;
  /// In the party's table, gMainRoutineState.holdings.coins; not named here.
  u32 partyCoins;
  struct DjinnRecoveryList list;
};

#endif
