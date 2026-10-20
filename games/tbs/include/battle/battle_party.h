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

/// A Djinni on standby or recovering: its element and index, its owner, and
/// the turns until it is set again.
struct DjinnRecoveryEntry {
  u8 element; ///< enum Element
  u8 index;   ///< the Djinni within its element, 0 to 19
  u8 unitId;  ///< its owner
  /// Turns until it is set again; -1 on standby
  /// (DJINN_RECOVERY_AFTER_BATTLE), -2 while a summon uses it.
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
