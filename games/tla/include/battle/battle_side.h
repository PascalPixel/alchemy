#ifndef ALCHEMY_BATTLE_SIDE_H
#define ALCHEMY_BATTLE_SIDE_H

/// @file
/// The two sides of a battle, their unit lists, and the Djinn each side has on
/// standby or recovering.

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

/// Which sides a unit list takes.
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
/// (DJINN_RECOVERY_SUMMONED), both in BattleActionRun_Build.
#define DJINN_RECOVERY_AFTER_BATTLE -1

/// DjinnRecoveryEntry.turns of a standby Djinni a summon has just used, until
/// the summoner's action ends and sets its count of turns to recover.
#define DJINN_RECOVERY_SUMMONED -2

/// A Djinni on standby or recovering: its element and index, its owner, and
/// the turns until it is set again.
struct DjinnRecoveryEntry {
  u8 element; ///< enum Element
  u8 index;   ///< the Djinni within its element
  u8 unitId;  ///< its owner
  s8 turns;   ///< turns until it is set again; -1 on standby, -2 in use
};

STATIC_ASSERT(sizeof(struct DjinnRecoveryEntry) == 4);

/// One side's Djinn recovery list: up to 72 entries and their count.
struct DjinnRecoveryList {
  struct DjinnRecoveryEntry entries[72];
  s32 count; ///< entries in use
};

/// One side's Djinn recovery table, as Djinn_GetRecoveryTable returns it: the
/// party's inside gMainRoutineState from summonMask on, the link opponent's in
/// the fourth enemy unit (FIRST_ENEMY_UNIT + 3), which an opponent of three
/// never fills.
struct DjinnRecoveryTable {
  /// The summons the side can call, bit n for summon n (summon_table.c).
  u32 summonMask;
  u32 partyCoins; ///< in the party's table, gMainRoutineState.coins
  struct DjinnRecoveryList list;
};

s32 BattleParty_PrepareMembers(u16 *owners);
s32 BattleEnemy_List(s16 *unitIds);
s32 BattleUnit_ListLiving(s32 sideMask, u16 *unitIds);
s32 BattleParty_AssignOpponentSlots(void);
s32 BattleUnit_ListIds(s32 sideMask, u16 *unitIds);

s32 BattleUnit_RollCounterEarlyEnd(s32 unitId, s32 turns, s32 bias);
s32 BattleUnit_CountDownAttackStage(s32 unitId);
s32 BattleUnit_CountDownDefenseStage(s32 unitId);
s32 BattleUnit_CountDownResistanceStage(s32 unitId);
s32 BattleUnit_CountDownDelusion(s32 unitId);
s32 BattleUnit_CountDownConfusion(s32 unitId);
s32 BattleUnit_CountDownCharm(s32 unitId);
s32 BattleUnit_CountDownStun(s32 unitId);
s32 BattleUnit_CountDownSleep(s32 unitId);
s32 BattleUnit_CountDownPsynergySeal(s32 unitId);
s32 BattleUnit_CountDownReflect(s32 unitId);
s32 BattleUnit_CountDownAgilityStage(s32 unitId);
s32 DjinnRecovery_CountDown(void);
s32 DjinnRecovery_CountDownTwenty(void);

#endif
