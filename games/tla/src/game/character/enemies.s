	.section .rodata
	.balign 4
@ 敵 380 体
	.global gEnemy_DefinitionTable
gEnemy_DefinitionTable:
	.incbin "games/tla/src/game/character/enemies.table"
