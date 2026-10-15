#ifndef ALCHEMY_BATTLE_TYPE_H
#define ALCHEMY_BATTLE_TYPE_H

#include "layout_guard.h"
#include "battle_unit.h"

enum
{
	BATTLE_TARGET_CAPACITY = 14,
	BATTLE_PLAN_SIZE = 0x64
};

struct BattlePlan
{
	u8	aid;
	s8	target_count;
	u8	target_ids[BATTLE_TARGET_CAPACITY];
	s8	target_dist[BATTLE_TARGET_CAPACITY];
	s8	target_adj[BATTLE_TARGET_CAPACITY];
	s8	target_crit[BATTLE_TARGET_CAPACITY];
	s8	target_hit[BATTLE_TARGET_CAPACITY];
	s16 cmd;
	u8	unknown_4a[2];
	s32 ability_id;
	s32 element;
	s32 outcome;
	u32 presentation_flags;
	s32 failure;
	u32 haunt_dmg;
};

LAYOUT_SIZE_GUARD(BattlePlan_Size, struct BattlePlan, BATTLE_PLAN_SIZE);
LAYOUT_OFFSET_GUARD(BattlePlan_TargetIds, struct BattlePlan, target_ids, 0x02);
LAYOUT_OFFSET_GUARD(BattlePlan_TargetAdj, struct BattlePlan, target_adj, 0x1e);
LAYOUT_OFFSET_GUARD(BattlePlan_ActionId, struct BattlePlan, ability_id, 0x4c);
LAYOUT_OFFSET_GUARD(BattlePlan_SpiritDmg, struct BattlePlan, haunt_dmg, 0x60);

#endif
