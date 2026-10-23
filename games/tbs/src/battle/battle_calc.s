@ Percentages by distance from the chosen target (0 is the target itself),
@ the same in both games: each game's battle_data.s includes them in place.
@ Hit rate (%)
	.global gRangeFalloff_EffectChance
gRangeFalloff_EffectChance:
	.byte 100, 60, 30, 30, 30, 30
	.balign 4
@ PP drained
	.global gRangeFalloff_PpLoss
gRangeFalloff_PpLoss:
	.4byte 100, 50, 30, 10, 10, 10
@ HP healed
	.global gRangeFalloff_HealHp
gRangeFalloff_HealHp:
	.4byte 100, 80, 60, 40, 20, 10
@ PP damage
	.global gRangeFalloff_PpDamage
gRangeFalloff_PpDamage:
	.4byte 100, 50, 30, 10, 10, 10
@ HP damage of the ability kinds ABILITY_FORMULA_DAMAGE, ABILITY_FORMULA_SUMMON and
@ ABILITY_FORMULA_ENEMY_DAMAGE, in that order
	.global gRangeFalloff_PsynergyDamage
gRangeFalloff_PsynergyDamage:
	.4byte 100, 80, 60, 40, 20, 10
	.global gRangeFalloff_SummonDamage
gRangeFalloff_SummonDamage:
	.4byte 100, 70, 40, 30, 20, 10
	.global gRangeFalloff_EnemyDamage
gRangeFalloff_EnemyDamage:
	.4byte 100, 50, 30, 10, 10, 10
@ PP restored
	.global gRangeFalloff_HealPp
gRangeFalloff_HealPp:
	.4byte 100, 80, 60, 40, 20, 10
@ HP damage
	.global gRangeFalloff_EffectDamage
gRangeFalloff_EffectDamage:
	.4byte 100, 50, 30, 10, 10, 10
