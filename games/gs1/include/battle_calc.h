#ifndef ALCHEMY_BATTLE_CALC_H
#define ALCHEMY_BATTLE_CALC_H

#include "types.h"

/*
 * target_offsets の絶対値で引く百分率。
 * 中心から離れるほど効果が落ちる。
 */

extern u8 Data_080c2ab8[];
extern s32 Data_080c2ac0[];
extern s32 Data_080c2ad8[];
extern s32 Data_080c2af0[];
extern s32 Data_080c2b08[];
extern s32 Data_080c2b20[];
extern s32 Data_080c2b38[];
extern s32 Data_080c2b50[];
extern s32 Data_080c2b68[];

#define HitFalloff Data_080c2ab8
#define PpLossFalloff Data_080c2ac0
#define HpHealFalloff Data_080c2ad8
#define PpDmgFalloff Data_080c2af0
#define HpDmgFalloff5 Data_080c2b08
#define HpDmgFalloff8 Data_080c2b20
#define HpDmgFalloff6 Data_080c2b38
#define PpHealFalloff Data_080c2b50
#define HpDmgFalloff Data_080c2b68

/* Shared arithmetic; callers must avoid divide-by-zero. */

s32 Func_080022ec(s32 numerator, s32 denominator);
s32 Func_080022f4(s32 numerator, s32 denominator);
#define Math_Div Func_080022ec
#define Math_Mod Func_080022f4
#define Battle_HitCheck Func_08077178
#define Battle_CalcAttack Func_08077180
#define Battle_CalcPower Func_08077188
#define Battle_CalcRestore Func_08077190
#define BattleUnit_Drain Func_08077120
#define BattleUnit_Assign Func_08077140
#define BattleEffect_OnDead Func_080bbae8

s32 Battle_HitCheck(s32 actor, s32 target, s32 range, s32 effect, s32 table);
s32 Battle_CalcAttack(s32 power, s32 scale, s32 unused, s32 bonus);
s32 Battle_CalcPower(s32 power, s32 bonus, s32 scale);
s32 Battle_CalcRestore(s32 power, s32 scale, s32 factor);
s16 BattleUnit_Drain(s32 unit, s32 amount);
s32 BattleUnit_Assign(s32 slot, s32 unit, s32 mask);
s32 BattleEffect_OnDead(s32 effect);

#endif
