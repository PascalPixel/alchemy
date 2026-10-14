	.section .rodata
	.balign 4
@ 敵 380 体
	.global Enemy_DefinitionTable
Enemy_DefinitionTable:
	.incbin "games/tla/src/game/character/enemies.table"
