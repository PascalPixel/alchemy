#ifndef ALCHEMY_RANGE_FALLOFF_H
#define ALCHEMY_RANGE_FALLOFF_H

/* How an ability weakens with each place a target stands from the aimed one,
   and the damage and recovery formulas that use it. */

#include "types.h"
#include "edition.h"

/* The range falloff: each table is a percent of the full amount, indexed by the
   target's distance in places from the aimed unit (0 to 5), from the chance an
   effect lands to the damage, healing and PP each formula gives. */
extern Uint8  RangeFalloffEffectChance[];
extern Sint32 RangeFalloffPpLoss[];
extern Sint32 RangeFalloffHealHp[];
extern Sint32 RangeFalloffPpDamage[];
extern Sint32 RangeFalloffPsynergyDamage[];
extern Sint32 RangeFalloffSummonDamage[];
extern Sint32 RangeFalloffEnemyDamage[];
extern Sint32 RangeFalloffHealPp[];
extern Sint32 RangeFalloffEffectDamage[];

/* Rolls whether an ability's effect lands on the target, less likely the
   farther it is from the aimed unit (gmBattleRollEffectSuccess); returns 1 when
   it does. */
Sint32 gmBattleRollEffectSuccessStub(
	Sint32 actor, Sint32 tgt, Sint32 range, Sint32 effect, Sint32 tbl);
/* Scales max(attack - defense, 0) plus twice the added power by half the
   element multiplier, then halves it (gmBattleCalcAttackDamage). */
Sint32 gmBattleCalcAttackDamageStub(
	Sint32 amount, Sint32 reduction, Sint32 adjustment, Sint32 selector);
/* Returns Psynergy's power times the element multiplier and powerMultiplier
   (gmBattleCalcPsynergyPower). */
Sint32 gmBattleCalcPsynergyPowerStub(
	Sint32 value, Sint32 selector, Sint32 multiplier);
/* Returns the amount healed: power times powerMultiplier and the multiplier for
   the healer's element power above 100 (gmBattleCalcRecovery). */
Sint32 gmBattleCalcRecoveryStub(
	Sint32 value, Sint32 selector, Sint32 multiplier);
/* Adds amount to the unit's PP, kept between 0 and its maximum, updates its
   gauges and returns the new PP. */
Sint16 gmUnitAddPpStub(Sint32 unit, Sint32 amount);
/* Fills a battle unit with an enemy: its stats, its name with its letter, its
   items, rewards and elements (gmBattleUnitSet). */
Sint32 gmBattleUnitSetStub(Sint32 unitId, Sint32 enemyId, Sint32 suffix);
Bool   btAbilityEffectAppliesToDowned(Sint32 effect);

#endif
