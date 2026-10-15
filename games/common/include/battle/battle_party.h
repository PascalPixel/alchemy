#ifndef ALCHEMY_BATTLE_PARTY_H
#define ALCHEMY_BATTLE_PARTY_H

#include "edition.h"
#include "layout_guard.h"

enum BattleUnitPresence
{
	BATTLE_UNIT_ABSENT = 0,
	BATTLE_UNIT_ENEMY = 1,
	BATTLE_UNIT_PARTY = 2
};

enum BattleUnitListMark
{
	BATTLE_UNIT_REMOVED = 0xfe,
	BATTLE_UNIT_LIST_END = 0xff
};

enum BattleSideMask
{
	BATTLE_SIDE_PARTY = 1,
	BATTLE_SIDE_ENEMIES = 2,
	BATTLE_SIDE_BOTH = BATTLE_SIDE_PARTY | BATTLE_SIDE_ENEMIES
};

struct DjinnRecoveryEntry
{
	u8 element;
	u8 index;
	u8 unit_id;
	s8 turns;
};

LAYOUT_SIZE_GUARD(DjinnRecoveryEntry_Size, struct DjinnRecoveryEntry, 4);

struct DjinnRecoveryList
{
	struct DjinnRecoveryEntry entries[64];
	s32						  count;
};

struct DjinnRecoveryTable
{
	u8						 unknown_00[8];
	struct DjinnRecoveryList list;
};

#if GAME_TLA
s32 BattleParty_PrepareActiveOwners(u16 *unit_ids);
s32 BtlEnmList(s16 *unit_ids);
s32 BattleParty_ListLivingUnits(s32 side_mask, u16 *unit_ids);
s32 BattleParty_AssignMemberSlots(void);
s32 BattleParty_ListActorIds(s32 side_mask, u16 *unit_ids);

s32 BattleUnit_RollCounterEarlyEnd(s32 unit_id, s32 turns, s32 bias);
s32 BattleUnit_TickAttackModifier(s32 unit_id);
s32 BattleUnit_TickDefenseModifier(s32 unit_id);
s32 BattleUnit_TickResistanceModifier(s32 unit_id);
s32 BattleUnit_TickDelusion(s32 unit_id);
s32 BattleUnit_TickConfusion(s32 unit_id);
s32 BattleUnit_TickCharm(s32 unit_id);
s32 BattleUnit_TickStun(s32 unit_id);
s32 BattleUnit_TickSleep(s32 unit_id);
s32 Battle_AdvanceCounterAndCheckChance(s32 unit_id);
s32 BattleUnit_TickReflect(s32 unit_id);
s32 BattleUnit_TickAgilityModifier(s32 unit_id);
s32 BattlePlacement_UpdateTimedEntries(void);
s32 BattlePlacement_UpdateTimedEntriesTwentyTimes(void);
#endif

#endif
