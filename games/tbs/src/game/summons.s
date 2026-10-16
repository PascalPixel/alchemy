	.section .rodata
@ Summon order.
	.global gSummon_OrderList
gSummon_OrderList:
	.byte 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
@ Definitions of the 16 summons.
	.global gSummon_DefinitionTable
gSummon_DefinitionTable:
	.incbin "games/tbs/src/game/summons.table"
