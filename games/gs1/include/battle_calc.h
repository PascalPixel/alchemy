#ifndef ALCHEMY_BATTLE_CALC_H
#define ALCHEMY_BATTLE_CALC_H

#include "types.h"

/*
 * target_offsets の絶対値で引く百分率。
 * 中心から離れるほど効果が落ちる。
 */
extern u8 Data_080c2ab8[];  /* 命中率 */
extern s32 Data_080c2ac0[]; /* PP喪失 */
extern s32 Data_080c2ad8[]; /* HP回復 */
extern s32 Data_080c2af0[]; /* PPダメージ */
extern s32 Data_080c2b08[]; /* HPダメージ 種別5 */
extern s32 Data_080c2b20[]; /* HPダメージ 種別8 */
extern s32 Data_080c2b38[]; /* HPダメージ 種別6 */
extern s32 Data_080c2b50[]; /* PP回復 */
extern s32 Data_080c2b68[]; /* HPダメージ 種別3 */

#define HitFalloff Data_080c2ab8
#define PpLossFalloff Data_080c2ac0
#define HpHealFalloff Data_080c2ad8
#define PpDmgFalloff Data_080c2af0
#define HpDmgFalloff5 Data_080c2b08
#define HpDmgFalloff8 Data_080c2b20
#define HpDmgFalloff6 Data_080c2b38
#define PpHealFalloff Data_080c2b50
#define HpDmgFalloff Data_080c2b68

/* ゼロ除算を避ける共有算術ルーチン。 */
s32 Func_080022ec(s32 numerator, s32 denominator);
s32 Func_080022f4(s32 numerator, s32 denominator);

#define Math_Div Func_080022ec
#define Math_Mod Func_080022f4

s32 Func_08077178(s32 actor, s32 target, s32 range, s32 effect, s32 table); /* 命中判定 */
s32 Func_08077180(s32 power, s32 scale, s32 unused, s32 bonus); /* 武器攻撃 */
s32 Func_08077188(s32 power, s32 bonus, s32 scale); /* 威力ベース */
s32 Func_08077190(s32 power, s32 scale, s32 factor); /* 回復量 */
s16 Func_08077120(s32 unit, s32 amount); /* 吸収分を行動側へ */
s32 Func_08077140(s32 slot, s32 unit, s32 mask); /* 召喚ユニットの配置 */
s32 Func_080bbae8(s32 effect); /* 倒れていても効く効果か */

#define Battle_HitCheck Func_08077178
#define Battle_CalcAttack Func_08077180
#define Battle_CalcPower Func_08077188
#define Battle_CalcRestore Func_08077190
#define BattleUnit_Drain Func_08077120
#define BattleUnit_Assign Func_08077140
#define BattleEffect_OnDead Func_080bbae8

#endif
