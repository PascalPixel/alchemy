#ifndef ALCHEMY_RANGE_FALLOFF_H
#define ALCHEMY_RANGE_FALLOFF_H

#include "types.h"
#include "edition.h"

extern u8 gRangeFalloff_EffectChance[];
extern s32 gRangeFalloff_PpLoss[];
extern s32 gRangeFalloff_HealHp[];
extern s32 gRangeFalloff_PpDamage[];
extern s32 gRangeFalloff_PsynergyDamage[];
extern s32 gRangeFalloff_SummonDamage[];
extern s32 gRangeFalloff_EnemyDamage[];
extern s32 gRangeFalloff_HealPp[];
extern s32 gRangeFalloff_EffectDamage[];

s32 Battle_RollEffectSuccessFar(s32 actor, s32 target, s32 range, s32 effect,
                                s32 table);
s32 Battle_CalculateAttackDamageFar(s32 amount, s32 reduction, s32 adjustment,
                                    s32 selector);
s32 Battle_CalculatePsynergyPowerFar(s32 value, s32 selector, s32 multiplier);
s32 Battle_CalculateRecoveryFar(s32 value, s32 selector, s32 multiplier);
s16 Unit_AddPpFar(s32 unit, s32 amount);
s32 BattleUnit_SetFar(s32 slot, s32 unit, s32 mask);
s32 Ability_EffectAppliesToDowned(s32 effect);

#endif
