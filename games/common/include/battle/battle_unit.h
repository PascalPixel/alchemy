#ifndef ALCHEMY_BATTLE_UNIT_H
#define ALCHEMY_BATTLE_UNIT_H

#include "edition.h"
#include "layout_guard.h"

enum
{
	BATTLE_ACTION_SIZE = 0x10,
	BATTLE_UNIT_SIZE = 0x14c,
	OWNER_ACTION_ID_MASK = 0x3fff
};

struct OwnerActionSlot
{
	u16 encoded_action;
	u16 unknown_02;
};

struct BattleElementStats
{
	s16 pow;
	s16 res;
};

struct BtlAct
{
	u8	target_mode;
	u8	tgt_flg;
	u8	damage_class;
	u8	efx;
	u8	unknown_04[4];
	u8	rng;
	u8	pp_cost;
	u16 pow;
	u8	type_0c;
	u8	unknown_0d[3];
};

enum BattleActionUse
{
	ACTION_USABLE_IN_FIELD = 0x40,
	ACTION_USABLE_IN_BATTLE = 0x80
};

struct BtlUnit
{
	u8						  name[0x0f];
	u8						  level;
	s16						  base_hp;
	s16						  base_ep;
	s16						  hp_gauge;
	s16						  ep_gauge;
	u16						  base_attack;
	u16						  base_defense;
	u16						  base_agility;
	u8						  base_luck;
	u8						  base_action_count;
	u8						  base_hp_regen;
	u8						  base_ep_regen;
	u8						  unknown_022[2];
	struct BattleElementStats base_elm[4];
	s16						  max_hp;
	s16						  max_ep;
	s16						  hp;
	s16						  ep;
	u16						  atk;
	u16						  def;
	u16						  agi;
	u8						  luck;
	u8						  action_entry_count;
	u8						  hp_regen;
	u8						  ep_regen;
	u8						  unknown_046[2];
	struct BattleElementStats elm[4];
	struct OwnerActionSlot	  action_slots[32];
	u16						  inventory[15];
	u8						  unknown_0f6[2];
	u32						  djinn_available[4];
	u32						  djinn_active[4];
	u8						  djinn_owned_counts[4];
	u8						  djinn_active_counts[4];
	u32						  rewards;
	u32						  experience;
	u8						  class_id;
	u8						  class_index;
	u8						  side;
#if GAME_TLA
	u8 guard_lv;
#else
	s8 guard_lv;
#endif
	s8	elm_lv[4];
	s8	restraint;
	s8	doku;
	u8	atk_cnt;
	s8	atk_lv;
	u8	def_cnt;
	s8	def_lv;
	u8	res_cnt;
	s8	res_lv;
	u8	maboroshi;
	s8	konran;
	u8	charm;
	u8	shibire;
	u8	nemuri;
	u8	fuuji;
	u8	refrain;
	s8	reflect;
	u8	akuryo;
	u8	shiryo;
	u8	unknown_142[2];
	u8	sonae;
	u8	miugoki;
	u8	agi_cnt;
	s8	agi_lv;
	s8	chouhatsu;
	u8	unknown_149;
	u16 enemy_id;
};

LAYOUT_SIZE_GUARD(BattleAction_Size, struct BtlAct, BATTLE_ACTION_SIZE);
LAYOUT_OFFSET_GUARD(BattleAction_Effect, struct BtlAct, efx, 0x03);
LAYOUT_OFFSET_GUARD(BattleAction_Power, struct BtlAct, pow, 0x0a);
LAYOUT_OFFSET_GUARD(BattleAction_Type, struct BtlAct, type_0c, 0x0c);

LAYOUT_SIZE_GUARD(OwnerActionSlot_Size, struct OwnerActionSlot, 4);
LAYOUT_SIZE_GUARD(BattleElementStats_Size, struct BattleElementStats, 4);
LAYOUT_SIZE_GUARD(BattleUnit_Size, struct BtlUnit, BATTLE_UNIT_SIZE);
LAYOUT_OFFSET_GUARD(BattleUnit_BaseHp, struct BtlUnit, base_hp, 0x10);
LAYOUT_OFFSET_GUARD(BattleUnit_BaseAttack, struct BtlUnit, base_attack, 0x18);
LAYOUT_OFFSET_GUARD(BattleUnit_Actions, struct BtlUnit, action_slots, 0x58);
LAYOUT_OFFSET_GUARD(BattleUnit_AvailableDjinn, struct BtlUnit, djinn_available, 0xf8);
LAYOUT_OFFSET_GUARD(BattleUnit_ActiveDjinn, struct BtlUnit, djinn_active, 0x108);
LAYOUT_OFFSET_GUARD(BattleUnit_Rewards, struct BtlUnit, rewards, 0x120);
LAYOUT_OFFSET_GUARD(BattleUnit_Hp, struct BtlUnit, hp, 0x038);
LAYOUT_OFFSET_GUARD(BattleUnit_BaseElements, struct BtlUnit, base_elm, 0x024);
LAYOUT_OFFSET_GUARD(BattleUnit_Level, struct BtlUnit, level, 0x00f);
LAYOUT_OFFSET_GUARD(BattleUnit_Elements, struct BtlUnit, elm, 0x048);
LAYOUT_OFFSET_GUARD(BattleUnit_ReadyPose, struct BtlUnit, sonae, 0x144);
LAYOUT_OFFSET_GUARD(BattleUnit_Inventory, struct BtlUnit, inventory, 0x0d8);
LAYOUT_OFFSET_GUARD(BattleUnit_DjinnOwnedCounts, struct BtlUnit, djinn_owned_counts, 0x118);
LAYOUT_OFFSET_GUARD(BattleUnit_DjinnActiveCounts, struct BtlUnit, djinn_active_counts, 0x11c);
LAYOUT_OFFSET_GUARD(BattleUnit_GuardLevel, struct BtlUnit, guard_lv, 0x12b);
LAYOUT_OFFSET_GUARD(BattleUnit_Status13a, struct BtlUnit, charm, 0x13a);
LAYOUT_OFFSET_GUARD(BattleUnit_Status140, struct BtlUnit, akuryo, 0x140);

LAYOUT_OFFSET_GUARD(BattleUnit_EnemyId, struct BtlUnit, enemy_id, 0x14a);

// ゲージの割合 (0x4000 で満タン) を 0〜0x4000 に収める
static __inline__ s32 Vitals_Clamp(s32 ratio)
{
	s32 clamped;

	if (ratio > 0x4000)
		clamped = 0x4000;
	else if (ratio < 0)
		clamped = 0;
	else
		clamped = ratio;
	return clamped;
}

// HP と EP のゲージを計算し直す（残っていれば最低 1）
// A statement macro, written do { } while (0): every site matches through
// that shape. Written out at each site, or as an inline function, the
// gauge loads are scheduled across the store before them.
#define Unit_UpdateGauges(unit)                                               \
	do                                                                        \
	{                                                                         \
		(unit)->hp_gauge = Vitals_Clamp(((unit)->hp << 14) / (unit)->max_hp); \
		if ((unit)->hp_gauge == 0 && (unit)->hp != 0)                         \
			(unit)->hp_gauge = 1;                                             \
		(unit)->ep_gauge = Vitals_Clamp(((unit)->ep << 14) / (unit)->max_ep); \
		if ((unit)->ep_gauge == 0 && (unit)->ep != 0)                         \
			(unit)->ep_gauge = 1;                                             \
	} while (0)

#endif
