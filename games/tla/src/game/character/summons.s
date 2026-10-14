	.section .rodata
@ 召喚の順
	.global Summon_OrderList
Summon_OrderList:
	.byte 0, 1, 2, 3, 4, 5, 6, 7, 16, 17, 8, 9, 10, 11, 18, 19
	.byte 12, 13, 14, 15, 20, 21, 22, 23, 24, 25, 26, 27, 28
	.balign 4, 0
@ 召喚 29 種
	.global Summon_DefinitionTable
Summon_DefinitionTable:
	.incbin "games/tla/src/game/character/summons.table"
