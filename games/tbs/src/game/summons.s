	.section .rodata
@ 召喚の順
	.global Summon_OrderList
Summon_OrderList:
	.byte 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
@ 召喚 16 種
	.global Summon_DefinitionTable
Summon_DefinitionTable:
	.incbin "games/tbs/src/game/summons.table"
