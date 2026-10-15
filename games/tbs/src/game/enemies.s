	.section .rodata
	.balign 4
@ 敵 165 体
	.global gEnemy_Definitions
gEnemy_Definitions:
	.incbin "games/tbs/src/game/enemies.table"
