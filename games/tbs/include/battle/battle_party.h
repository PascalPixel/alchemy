#ifndef ALCHEMY_BATTLE_PARTY_H
#define ALCHEMY_BATTLE_PARTY_H

/// @file
/// Which units are in a battle, and the Djinn recovery list.

#include "edition.h"
#include "static_assert.h"

/// BattleUnit.side: which side a unit fights on, if it is in the battle.
enum BattleUnitPresence {
  BATTLE_UNIT_ABSENT = 0,
  BATTLE_UNIT_ENEMY = 1,
  BATTLE_UNIT_PARTY = 2
};

/// Marks in a list of unit ids, such as BattleWork.partyUnits and enemyIds.
enum BattleUnitListMark {
  BATTLE_UNIT_REMOVED = 0xfe, ///< a place whose unit has left the battle
  BATTLE_UNIT_LIST_END = 0xff ///< ends the list
};

/// Which sides a unit list takes, as BattleParty_ListLivingUnits reads it.
enum BattleSideMask {
  BATTLE_SIDE_PARTY = 1,
  BATTLE_SIDE_ENEMIES = 2,
  BATTLE_SIDE_BOTH = BATTLE_SIDE_PARTY | BATTLE_SIDE_ENEMIES
};

/// The Djinn recovery list's two tables (Djinn_GetRecoveryTableFar): the
/// party's and the link opponent's.
#define DJINN_RECOVERY_PARTY 0
#define DJINN_RECOVERY_OPPONENT 1

/// DjinnRecoveryEntry.turns of a Djinni on standby. In battle it stays so
/// until a Djinn command sets it again or a summon uses it
/// (DJINN_RECOVERY_SUMMONED), both in
/// BattleCommand_BuildPlan; an owner who does not fight has it set as the
/// next battle starts, outside a link battle
/// (BattleParty_ReconcileDjinnRecovery). DjinnRecovery_Tick counts down only
/// positive turns.
#define DJINN_RECOVERY_AFTER_BATTLE -1

/// DjinnRecoveryEntry.turns of a standby Djinni a summon has just used, until
/// the turn's end gives it its turn to recover (BattleUnit_ProcessTurnEnd).
#define DJINN_RECOVERY_SUMMONED -2

/// A Djinni on standby or recovering: its element and index, its owner, and
/// the turns until it is set again.
struct DjinnRecoveryEntry {
  u8 element; ///< enum Element
  u8 index;   ///< the Djinni within its element
  u8 unitId;  ///< its owner
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
/// party's inside gGameState from its byte 12, the enemies' over battle unit
/// 0x83's record.
struct DjinnRecoveryTable {
  u8 unk00[8]; ///< its first word is BattleSummonState's availableMask
  struct DjinnRecoveryList list;
};

#endif
