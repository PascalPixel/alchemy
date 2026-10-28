@ Percentages by distance from the chosen target (0 is the target itself),
@ the same in both games: each game's battle_data.s includes them in place.
@ Hit rate (%)
	.global gHitTable
gHitTable:
	.byte 100, 60, 30, 30, 30, 30
	.balign 4
@ PP drained
	.global gPpLossTable
gPpLossTable:
	.4byte 100, 50, 30, 10, 10, 10
@ HP healed
	.global gHpRecoveryTable
gHpRecoveryTable:
	.4byte 100, 80, 60, 40, 20, 10
@ PP damage
	.global gPpDmgTable
gPpDmgTable:
	.4byte 100, 50, 30, 10, 10, 10
@ HP damage of effect types 5, 8 and 6
	.global gHpDmgTable5
gHpDmgTable5:
	.4byte 100, 80, 60, 40, 20, 10
	.global gHpDmgTable8
gHpDmgTable8:
	.4byte 100, 70, 40, 30, 20, 10
	.global gHpDmgTable6
gHpDmgTable6:
	.4byte 100, 50, 30, 10, 10, 10
@ PP restored
	.global gPpRecoveryTable
gPpRecoveryTable:
	.4byte 100, 80, 60, 40, 20, 10
@ HP damage
	.global gHpDmgTable
gHpDmgTable:
	.4byte 100, 50, 30, 10, 10, 10
