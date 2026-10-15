//--------------------------------------------------------------------------
// random16.c
// バトル：乱数と、ダメージ・回復量の計算
//--------------------------------------------------------------------------

#include "types.h"
#include "battle.h"
#include "game.h"
#include "system.h"
#include "battle_calc.h"
#include "scene.h"
#include "item.h"
#include "inventory.h"
#include "battle_random.h"
#include "battle_type.h"

//--------------------------------------------------------------------------
// バトル用の乱数（線形合同法）。種の 8-23 ビット目の 16 ビットを返す
//--------------------------------------------------------------------------
u32 BtlRnd(void)
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
	return (BtlRnd() * 100) >> 16;
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

s32 Owner_GetDefaultElement(void *state);

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
	item = Inventory_GetEquippedDefinition((void *)unit, 1); // 1 : 武器
	if (item != NULL)
		return item->element;
	return 4;
}
