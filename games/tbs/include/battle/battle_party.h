#ifndef ALCHEMY_BATTLE_PARTY_H
#define ALCHEMY_BATTLE_PARTY_H

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

struct DjinnRecoveryEntry {
  u8 element;
  u8 index;
  u8 unitId;
  s8 turns;
};

STATIC_ASSERT(sizeof(struct DjinnRecoveryEntry) == 4);

struct DjinnRecoveryList {
  struct DjinnRecoveryEntry entries[64];
  s32 count;
};

struct DjinnRecoveryTable {
  u8 unk00[8];
  struct DjinnRecoveryList list;
};

#endif
