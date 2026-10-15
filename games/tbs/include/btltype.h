#ifndef ALCHEMY_BATTLE_TYPES_H
#define ALCHEMY_BATTLE_TYPES_H

#include "layout_guard.h"
#include "battle_unit.h"

enum
{
	BATTLE_TARGET_CAPACITY = 14,
	BATTLE_PLAN_SIZE = 0x64
};

struct BtlPlan
{
	u8	aid;
	s8	target_count;
	u8	tgt_no[BATTLE_TARGET_CAPACITY];
	s8	tgt_dist[BATTLE_TARGET_CAPACITY];
	s8	tgt_adj[BATTLE_TARGET_CAPACITY];
	s8	tgt_crit[BATTLE_TARGET_CAPACITY];
	s8	tgt_hit[BATTLE_TARGET_CAPACITY];
	s16 cmd;
	u8	unknown_4a[2];
	s32 act_no;
	s32 elm;
	s32 outcome;
	u32 presentation_flags;
	s32 failure;
	u32 haunt_dmg;
};

LAYOUT_SIZE_GUARD(BattlePlan_Size, struct BtlPlan, BATTLE_PLAN_SIZE);
LAYOUT_OFFSET_GUARD(BattlePlan_TgtNo, struct BtlPlan, tgt_no, 0x02);
LAYOUT_OFFSET_GUARD(BattlePlan_TgtAdj, struct BtlPlan, tgt_adj, 0x1e);
LAYOUT_OFFSET_GUARD(BattlePlan_ActionId, struct BtlPlan, act_no, 0x4c);
LAYOUT_OFFSET_GUARD(BattlePlan_SpiritDmg, struct BtlPlan, haunt_dmg, 0x60);

#endif
