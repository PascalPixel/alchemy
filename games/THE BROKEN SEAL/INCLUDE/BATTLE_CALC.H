#ifndef ALCHEMY_BATTLE_CALC_H
#define ALCHEMY_BATTLE_CALC_H

#include "TYPES.H"

/*
 * target_offsets の絶対値で引く百分率。
 * 中心から離れるほど効果が落ちる。
 */

extern u8 HitFalloff[];
extern s32 PpLossFalloff[];
extern s32 HpHealFalloff[];
extern s32 PpDmgFalloff[];
extern s32 HpDmgFalloff5[];
extern s32 HpDmgFalloff8[];
extern s32 HpDmgFalloff6[];
extern s32 PpHealFalloff[];
extern s32 HpDmgFalloff[];

/* Shared arithmetic; callers must avoid divide-by-zero. */

s32 Math_Div(s32 numerator, s32 denominator);
s32 Math_DivU(s32 numerator, s32 denominator);

s32 Battle_HitCheck(s32 actor, s32 target, s32 range, s32 effect, s32 table);
s32 Battle_CalcAttack(s32 power, s32 scale, s32 unused, s32 bonus);
s32 Battle_CalcPower(s32 power, s32 bonus, s32 scale);
s32 Battle_CalcRestore(s32 power, s32 scale, s32 factor);
s16 Owner_AdjustSecondValueFar(s32 unit, s32 amount);
s32 BattleUnit_AssignFar(s32 slot, s32 unit, s32 mask);
s32 BattleFx_CanAffectDefeatedUnit(s32 effect);

#endif
