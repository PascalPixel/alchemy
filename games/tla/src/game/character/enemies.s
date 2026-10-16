	.section .rodata
	.balign 4
@ 380 enemies.
	.global gEnemy_DefinitionTable
gEnemy_DefinitionTable:
	.incbin "games/tla/src/game/character/enemies.table"
