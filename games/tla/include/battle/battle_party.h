#ifndef ALCHEMY_BATTLE_PARTY_H
#define ALCHEMY_BATTLE_PARTY_H

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

/// Which sides a unit list takes.
enum BattleSideMask {
  BATTLE_SIDE_PARTY = 1,
  BATTLE_SIDE_ENEMIES = 2,
  BATTLE_SIDE_BOTH = BATTLE_SIDE_PARTY | BATTLE_SIDE_ENEMIES
};

/// A Djinni on standby or recovering: its element and index, its owner, and
/// the turns until it is set again.
struct DjinnRecoveryEntry {
  u8 element; ///< enum Element
  u8 index;   ///< the Djinni within its element
  u8 unitId;  ///< its owner
  s8 turns;   ///< turns until it is set again; -1 on standby, -2 in use
};

STATIC_ASSERT(sizeof(struct DjinnRecoveryEntry) == 4);

/// One side's Djinn recovery list: up to 64 entries and their count.
struct DjinnRecoveryList {
  struct DjinnRecoveryEntry entries[64];
  s32 count; ///< entries in use
};

/// One side's Djinn recovery table.
struct DjinnRecoveryTable {
  u8 unk00[8]; ///< its first word is BattleSummonState's availableMask
  struct DjinnRecoveryList list;
};

s32 BattleParty_PrepareActiveOwners(u16 *unit_ids);
s32 BattleEnemy_List(s16 *unit_ids);
s32 BattleParty_ListLivingUnits(s32 side_mask, u16 *unit_ids);
s32 BattleParty_AssignMemberSlots(void);
s32 BattleParty_ListActorIds(s32 side_mask, u16 *unit_ids);

s32 BattleUnit_RollCounterEarlyEnd(s32 unitId, s32 turns, s32 bias);
s32 BattleUnit_CountDownAttackModifier(s32 unitId);
s32 BattleUnit_CountDownDefenseModifier(s32 unitId);
s32 BattleUnit_CountDownResistanceModifier(s32 unitId);
s32 BattleUnit_CountDownDelusion(s32 unitId);
s32 BattleUnit_CountDownConfusion(s32 unitId);
s32 BattleUnit_CountDownCharm(s32 unitId);
s32 BattleUnit_CountDownStun(s32 unitId);
s32 BattleUnit_CountDownSleep(s32 unitId);
s32 Battle_AdvanceCounterAndCheckChance(s32 unitId);
s32 BattleUnit_CountDownReflect(s32 unitId);
s32 BattleUnit_CountDownAgilityModifier(s32 unitId);
s32 DjinnRecovery_CountDown(void);
s32 DjinnRecovery_CountDownTwenty(void);

#endif
