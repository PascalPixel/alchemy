@ Percentages by distance from the chosen target (0 is the target itself),
@ the same in both games: each game's battle_data.s includes them in place.
@ Hit rate (%)
	.global HitTable
HitTable:
	.byte 100, 60, 30, 30, 30, 30
	.balign 4
@ PP drained
	.global PpLossTable
PpLossTable:
	.4byte 100, 50, 30, 10, 10, 10
@ HP healed
	.global HpRecoveryTable
HpRecoveryTable:
	.4byte 100, 80, 60, 40, 20, 10
@ PP damage
	.global PpDmgTable
PpDmgTable:
	.4byte 100, 50, 30, 10, 10, 10
@ HP damage of effect types 5, 8 and 6
	.global HpDmgTable5
HpDmgTable5:
	.4byte 100, 80, 60, 40, 20, 10
	.global HpDmgTable8
HpDmgTable8:
	.4byte 100, 70, 40, 30, 20, 10
	.global HpDmgTable6
HpDmgTable6:
	.4byte 100, 50, 30, 10, 10, 10
@ PP restored
	.global PpRecoveryTable
PpRecoveryTable:
	.4byte 100, 80, 60, 40, 20, 10
@ HP damage
	.global HpDmgTable
HpDmgTable:
	.4byte 100, 50, 30, 10, 10, 10
