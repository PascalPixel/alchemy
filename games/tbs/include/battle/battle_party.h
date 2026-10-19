#ifndef ALCHEMY_BATTLE_PARTY_H
#define ALCHEMY_BATTLE_PARTY_H

/// @file
/// Which units are in a battle, and the Djinn recovery list.

#include "edition.h"
#include "static_assert.h"

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

/// A Djinni on standby or recovering: its element and index, its owner, and
/// the turns until it is set again.
struct DjinnRecoveryEntry {
  u8 element;
  u8 index;
  u8 unitId;
  s8 turns;
};

STATIC_ASSERT(sizeof(struct DjinnRecoveryEntry) == 4);

/// One side's Djinn recovery list: up to 64 entries and their count.
struct DjinnRecoveryList {
  struct DjinnRecoveryEntry entries[64];
  s32 count;
};

/// One side's Djinn recovery table, as Djinn_GetRecoveryTable returns it.
struct DjinnRecoveryTable {
  u8 unk00[8];
  struct DjinnRecoveryList list;
};

#endif
