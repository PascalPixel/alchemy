#include "unit/game/character/starteq.h"
#include "unit/game/character/chara.h"
#include "unit/game/character/chara2.h"
#include "unit/game/character/chara3.h"
#include "unit/game/character/chara4.h"
#include "unit/game/character/chara5.h"
#include "unit/game/character/chara6.h"
#include "unit/game/character/chara7.h"
#include "unit/game/character/chara8.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime7.h"
#include "unit/system/scheduler/runtime8.h"
#include "unit/system/scheduler/runtime9.h"
#include "unit/system/scheduler/runtim10.h"
#include "unit/system/scheduler/runtim11.h"
#include "unit/system/scheduler/runtim13.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "mem.h"
#include "scene.h"
#include "owner_state.h"
#include "character.h"
#include "party_state.h"
#include "runtime_interfaces.h"
#include "item.h"
#include "game_flags.h"
#include "inventory.h"
#include "resource.h"
#include "resource_ids.h"
#include "dma.h"
#include "game_state.h"
#include "frame.h"
#include "save_state.h"
#include "io_reg.h"
#include "fixed_math.h"
#include "battle_unit.h"
#include "lib.h"
#include "edition.h"
#include "btlsys.h"
#include "text/msg_ids.h"
#include "btlsmn.h"
#include "preset_table.h"
#include "iwram_call.h"
#include "btlcalc.h"
#include "battle_random.h"
#include "btltype.h"
#include "battle_effect_chance.h"
#include "btlpty.h"
#include "field_event.h"

s32 Curve_LookupScaledValue(s32 input, s32 halve)
{
	s32 min;
	s32 max;
	s32 n;
	s32 i;
	s32 value;
	s32 dx;
	s32 dy;

	max = Element_PowerResistCurve[0][0];
	min = Element_PowerResistCurve[4][0];
	n = 5;
	if (input > max)
		input = max;
	else if (input < min)
		input = min;
	for (i = 0; i < n; i++)
	{
		if (input > Element_PowerResistCurve[i][0])
			break;
	}
	if (i == n)
	{
		value = Element_PowerResistCurve[i - 1][1];
	}
	else
	{
		dx = Element_PowerResistCurve[i - 1][0] - Element_PowerResistCurve[i][0];
		dy = Element_PowerResistCurve[i - 1][1] - Element_PowerResistCurve[i][1];
		value = (input - Element_PowerResistCurve[i][0]) * dy / dx + Element_PowerResistCurve[i][1];
	}
	switch (halve)
	{
	case 0:
		break;
	case 1:
		value /= 2;
		break;
	}
	return value + 0x100;
}

//--------------------------------------------------------------------------

// random16.c

// バトル：乱数と、ダメージ・回復量の計算

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// バトル用の乱数（線形合同法）。種の 8-23 ビット目の 16 ビットを返す
//--------------------------------------------------------------------------
s32 BtlRnd(void)
{
	s32 seed;

	seed = (gBattleRandomSeed * 0x41c64e6d) + 0x3039;
	gBattleRandomSeed = seed;
	return (u32)(seed << 8) >> 16;
}

//--------------------------------------------------------------------------
// 0 から 99 までの乱数
//--------------------------------------------------------------------------
u32 BattleRandomPercent(void)
{
	return ((u32)BtlRnd() * 100) >> 16;
}

//--------------------------------------------------------------------------
// こうげきのダメージ（BtlAtkCalc）
// (atk - def + add * 2) / 2 に、属性の差による倍率をかける
// 倍率は 256 で 1 倍。差の効きは BtlPowCalc の半分（128 から 384）
// atk   : こうげき
// def   : ぼうぎょ（atk - def が負なら 0 にする）
// add   : わざの威力
// bonus : 属性の差（力 - たいせい）
//--------------------------------------------------------------------------
s32 Owner_ScaleAdjustedAmount(s32 atk, s32 def, s32 add, s32 bonus)
{
	s32 dmg;
	s32 val;

	atk -= def;
	dmg = bonus;
	if (atk < 0)
		atk = 0;
	val = Curve_LookupScaledValue(dmg, 1) * (atk + (add * 2));
	if (val < 0)
		val += 0x1ff;
	dmg = val >> 9;
	if (dmg < 0)
		dmg = 0;
	return dmg;
}

//--------------------------------------------------------------------------
// エナジーの威力（BtlPowCalc）
// pow に属性の差による倍率（256 で 1 倍、0 から 512）と rate をかける
// pow   : 威力
// bonus : 属性の差（力 - たいせい）
// rate  : 倍率（256 で 1 倍）
//--------------------------------------------------------------------------
s32 Owner_ScaleValueByCurve(s32 pow, s32 bonus, s32 rate)
{
	s32 val;

	val = Curve_LookupScaledValue(bonus, 0) * pow * rate;
	if (val < 0)
		val = val + 0xffff;
	return val >> 16;
}

//--------------------------------------------------------------------------
// かいふくの量（BtlRcvCalc）
// 属性の力が 100 をこえた分の 2 倍で倍率（256 で 1 倍）を引き、
// pow と rate にかける
// pow  : 威力
// elm  : 行動側の属性の力（100 で 1 倍、200 以上で 2 倍、0 以下で 0）
// rate : 倍率（256 で 1 倍）
//--------------------------------------------------------------------------
s32 Owner_ScaleValueByOffsetCurve(s32 pow, s32 elm, s32 rate)
{
	s32 val;
	u32 prod;

	prod = Curve_LookupScaledValue(elm * 2 - 200, 0) * pow;
	prod = rate * prod;
	val = prod;
	if (val < 0)
		val = val + 0xffff;
	return val >> 16;
}

//--------------------------------------------------------------------------
// ユニットの攻撃の属性を返す（4 は属性なし）
// 敵は種類ごとの属性、味方は装備した武器の属性
// id : ユニット番号
//--------------------------------------------------------------------------
s32 Item_GetEquippedElement(s32 id)
{
	struct ItemDefinition *item;
	struct BtlUnit		  *unit;

	unit = UnitGet(id);
	if (unit->class_index == 0)
		return Owner_GetDefaultElement(unit);
	item = Inventory_GetEquippedDefinition(unit, 1); // 1 : 武器
	if (item != NULL)
		return item->element;
	return 4;
}

s32 Equipment_GetUnleashRateBonus(struct BtlUnit *owner)
{
	s32					   sum;
	s32					   i;
	s32					   j;
	struct ItemDefinition *item;

	sum = 0;
	for (i = 0; i < 15; i++)
	{
		if (owner->inventory[i] & 0x200)
		{
			item = Item_GetDirect(owner->inventory[i]);
			for (j = 0; j < 4; j++)
			{
				if (item->effects[j].kind == 23)
					sum += item->effects[j].amount;
			}
		}
	}
	if (sum < 0)
		sum = 0;
	return sum;
}

s32 RollWeaponUnleash(struct BtlUnit *owner)
{
	struct ItemDefinition *item;
	s32					   rate;

	if (owner->class_index == 0)
	{
		return 1;
	}
	item = Inventory_GetEquippedDefinition(owner, 1);
	if (item == NULL)
	{
		return 1;
	}
	if (item->description_message == 0)
	{
		return 1;
	}
	rate = ((Equipment_GetUnleashRateBonus(owner) + (item->secondary_flags * 5)) << 0x10) / 100;
	if (rate > (BtlRnd() & 0xFFFF))
	{
		return item->description_message;
	}
	return 1;
}

s32 BattleFx_GetBaseSuccessRate(s32 effect_id)
{
	s32 battle_result;
	u32 entry_index;

	entry_index = effect_id - 8;
	switch (entry_index)
	{
	case 4:
	case 5:
		return 0x46;
	case 8:
	case 9:
		return 0x4B;
	case 14:
		return 0x1E;
	case 15:
		return 0x28;
	case 16:
		return 0x2D;
	case 10:
	case 11:
	case 17:
		return 0x37;
	case 18:
		return 0x19;
	case 19:
		return 0x14;
	case 12:
	case 23:
		return 0x41;
	case 13:
	case 26:
		return 0x23;
	case 27:
		return 0x32;
	case 48:
		battle_result = 0x3C;
		goto block_18;
	case 49:
		battle_result = 0x5A;
		goto block_18;
	case 0:
	case 1:
	case 20:
	case 24:
		return 0x3C;
	default:
		battle_result = 0x64;
		break;
	}
block_18:
	return 0 - battle_result;
}

s32 BattleTarget_IsWeakToEffect(const struct BtlUnit *target, s32 effect_id)
{
	const u8 *entries;
	s32		  i;

	if (target->class_index == 0)
	{
		entries = Owner_GetRecord(target->class_id) + 0x48;
		for (i = 0; i < 3; i++)
		{
			if (*entries == effect_id)
				return 1;
			entries++;
		}
	}
	else
	{
		entries = Owner_GetRecordStride84(target->class_index)->unknown_50;
		for (i = 0; i < 3; i++)
		{
			if (*entries++ == effect_id)
				return 1;
		}
	}
	return 0;
}

s32 BtlEfxRevChk(s32 effect_id)
{
	if ((effect_id == 5) || (effect_id == 0x38) || (effect_id == 0x39))
	{
		return 1;
	}
	return 0;
}

s32 BattleFx_RollSuccess(
	s32 caster, s32 target, s32 resistance_category, s32 effect_id, s32 success_scale)
{
	/* FAKEMATCH: the byte-status view and pointer setup prevent adjacent reads from merging into wider loads. */
	u8			   *state = (u8 *)UnitGet(target);
	struct BtlUnit *unit = (struct BtlUnit *)state;
	s32				attempts = 1;
	s32				score;
	s32				attempt;
	s8			   *poison;
	u8			   *delusion;
	u8			   *confusion;
	u8			   *charm;
	u8			   *stun;
	u8			   *sleep;

	if (BtlEfxRevChk(effect_id) != 0 && unit->hp != 0)
	{
		return 0;
	}

	if (effect_id == 3 && *(s8 *)(state + (u32) & ((struct BtlUnit *)0)->doku) == 0)
	{
		goto fail;
	}

	goto action4_check;
action4_tail:
	if (state[(u32) & ((struct BtlUnit *)0)->shibire] == 0
		&& state[(u32) & ((struct BtlUnit *)0)->nemuri] == 0
		&& state[(u32) & ((struct BtlUnit *)0)->fuuji] == 0
		&& state[(u32) & ((struct BtlUnit *)0)->shiryo] == 0)
	{
		goto fail;
	}
	goto action4_done;
action4_check:
	if (effect_id == 4)
	{
		if (state[(u32) & ((struct BtlUnit *)0)->maboroshi] == 0
			&& state[(u32) & ((struct BtlUnit *)0)->konran] == 0
			&& state[(u32) & ((struct BtlUnit *)0)->charm] == 0)
		{
			goto action4_tail;
		}
	}

action4_done:
	poison = (s8 *)(state + (u32) & ((struct BtlUnit *)0)->doku);
	delusion = state + (u32) & ((struct BtlUnit *)0)->maboroshi;
	confusion = state + (u32) & ((struct BtlUnit *)0)->konran;
	charm = state + (u32) & ((struct BtlUnit *)0)->charm;
	stun = state + (u32) & ((struct BtlUnit *)0)->shibire;
	sleep = state + (u32) & ((struct BtlUnit *)0)->nemuri;
	if (effect_id == 0x40 && *poison == 0 && *delusion == 0 && *confusion == 0 && *charm == 0
		&& *stun == 0 && *sleep == 0 && state[(u32) & ((struct BtlUnit *)0)->fuuji] == 0
		&& state[(u32) & ((struct BtlUnit *)0)->shiryo] == 0
		&& state[(u32) & ((struct BtlUnit *)0)->akuryo] == 0)
	{
		return 0;
	}

	if (effect_id == 0x1C && state[(u32) & ((struct BtlUnit *)0)->shiryo] == 1)
	{
		return 0;
	}

	score = BattleFx_GetBaseSuccessRate(effect_id);
	if (score > 0)
	{
		s32 difference = Owner_GetResistanceValue(caster, resistance_category)
						 - Owner_GetResistanceValue(target, resistance_category)
						 - (unit->luck >> 1);
		score += difference * 3;
		if (BattleTarget_IsWeakToEffect(unit, effect_id) != 0)
		{
			score += 25;
		}
	}
	else
	{
		score = -score;
	}

	if (effect_id == 0x43)
	{
		attempts = 3;
	}

	for (attempt = 0; attempt < attempts; attempt++)
	{
		if (score * success_scale / 100 >= (s32)BattleRandomPercent())
		{
			return 1;
		}
	}
fail:
	return 0;
}
