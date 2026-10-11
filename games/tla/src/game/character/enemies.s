	.section .rodata
	.balign 4
@ 敵 380 体
	.global Enemy_DefinitionTable
Enemy_DefinitionTable:
	.incbin "game/character/enemies.table"
