	.section .rodata
	.balign 4
@ Definitions of the 165 enemies.
	.global gEnemy_Definitions
gEnemy_Definitions:
	.incbin "games/tbs/src/game/enemies.table"
