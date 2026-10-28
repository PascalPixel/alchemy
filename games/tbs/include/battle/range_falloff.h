#ifndef ALCHEMY_RANGE_FALLOFF_H
#define ALCHEMY_RANGE_FALLOFF_H

/// @file
/// The battle calculation tables: hit chances, HP and PP damage and recovery.

#include "types.h"
#include "edition.h"
#include "far_call.h"

// Percentages by a target's distance from the aimed position, 0 for the aimed
// target to 5 for the farthest: BattleAbility_ApplyToTarget scales an amount by
// the entry and divides by 100, so an ability spread over a group weakens step
// by step. gRangeFalloff_EffectChance instead scales the effect's chance,
// through Battle_RollEffectSuccessFar.
/// The share of an effect's chance to land (Battle_RollEffectSuccess's
/// successScale): 100, 60, then 30.
extern u8 gRangeFalloff_EffectChance[];
/// PP loss (ABILITY_FORMULA_PP_DAMAGE), Crazy Voice's and Psy Drain's: 100, 50,
/// 30, then 10.
extern s32 gRangeFalloff_PpLoss[];
/// HP healing (ABILITY_FORMULA_HEAL_HP): 100 down to 20 by 20, then 10.
extern s32 gRangeFalloff_HealHp[];
/// PP damage of the never-reached ability kind -1: 100, 50, 30, then 10.
extern s32 gRangeFalloff_PpDamage[];
/// Psynergy damage (ABILITY_FORMULA_DAMAGE): 100 down to 20 by 20, then 10.
extern s32 gRangeFalloff_PsynergyDamage[];
/// Summon damage (ABILITY_FORMULA_SUMMON): 100, 70, 40, 30, 20, then 10.
extern s32 gRangeFalloff_SummonDamage[];
/// An enemy's unleashed damage (ABILITY_FORMULA_ENEMY_DAMAGE): 100, 50, 30,
/// then 10.
extern s32 gRangeFalloff_EnemyDamage[];
/// PP healing (ABILITY_FORMULA_HEAL_PP): 100 down to 20 by 20, then 10.
extern s32 gRangeFalloff_HealPp[];
/// The damage of an effect ability whose effect lands (ABILITY_FORMULA_EFFECT):
/// 100, 50, 30, then 10.
extern s32 gRangeFalloff_EffectDamage[];

#endif
