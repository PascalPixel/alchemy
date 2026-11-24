#ifndef ALCHEMY_RANGE_FALLOFF_H
#define ALCHEMY_RANGE_FALLOFF_H

/// @file
/// How an ability weakens with each place a target stands from the aimed one,
/// and the damage and recovery formulas that use it.

#include "types.h"
#include "edition.h"

/// The range falloff: each table is a percent of the full amount, indexed by
/// the target's distance in places from the aimed unit (0 to 5), from the
/// chance an effect lands to the damage, healing and PP each formula gives.
extern u8 gRangeFalloff_EffectChance[];
extern s32 gRangeFalloff_PpLoss[];
extern s32 gRangeFalloff_HealHp[];
extern s32 gRangeFalloff_PpDamage[];
extern s32 gRangeFalloff_PsynergyDamage[];
extern s32 gRangeFalloff_SummonDamage[];
extern s32 gRangeFalloff_EnemyDamage[];
extern s32 gRangeFalloff_HealPp[];
extern s32 gRangeFalloff_EffectDamage[];

/// Rolls whether an ability's effect lands on the target, less likely the
/// farther it is from the aimed unit (Battle_RollEffectSuccess); returns 1 when
/// it does.
s32 Battle_RollEffectSuccessFar(s32 actor, s32 target, s32 range, s32 effect,
                                s32 table);
/// Scales max(attack - defense, 0) plus twice the added power by half the
/// element multiplier, then halves it (Battle_CalculateAttackDamage).
s32 Battle_CalculateAttackDamageFar(s32 amount, s32 reduction, s32 adjustment,
                                    s32 selector);
/// Returns Psynergy's power times the element multiplier and powerMultiplier
/// (Battle_CalculatePsynergyPower).
s32 Battle_CalculatePsynergyPowerFar(s32 value, s32 selector, s32 multiplier);
/// Returns the amount healed: power times powerMultiplier and the multiplier
/// for the healer's element power above 100 (Battle_CalculateRecovery).
s32 Battle_CalculateRecoveryFar(s32 value, s32 selector, s32 multiplier);
/// Adds amount to the unit's PP, kept between 0 and its maximum, updates its
/// gauges and returns the new PP.
s16 Unit_AddPpFar(s32 unit, s32 amount);
/// Fills a battle unit with an enemy: its stats, its name with its letter, its
/// items, rewards and elements (BattleUnit_Set).
s32 BattleUnit_SetFar(s32 unitId, s32 enemyId, s32 suffix);
s32 Ability_EffectAppliesToDowned(s32 effect);

#endif
