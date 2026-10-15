@ Percentages by distance from the chosen target (0 is the target itself),
@ the same in both games: each game's btldata.s includes them in place.
@ Hit rate (%)
	.global HitTbl
HitTbl:
	.byte 100, 60, 30, 30, 30, 30
	.balign 4
@ PP drained
	.global EpLossTbl
EpLossTbl:
	.4byte 100, 50, 30, 10, 10, 10
@ HP healed
	.global HpRcvTbl
HpRcvTbl:
	.4byte 100, 80, 60, 40, 20, 10
@ PP damage
	.global EpDmgTbl
EpDmgTbl:
	.4byte 100, 50, 30, 10, 10, 10
@ HP damage of effect types 5, 8 and 6
	.global HpDmgTbl5
HpDmgTbl5:
	.4byte 100, 80, 60, 40, 20, 10
	.global HpDmgTbl8
HpDmgTbl8:
	.4byte 100, 70, 40, 30, 20, 10
	.global HpDmgTbl6
HpDmgTbl6:
	.4byte 100, 50, 30, 10, 10, 10
@ PP restored
	.global EpRcvTbl
EpRcvTbl:
	.4byte 100, 80, 60, 40, 20, 10
@ HP damage
	.global HpDmgTbl
HpDmgTbl:
	.4byte 100, 50, 30, 10, 10, 10
