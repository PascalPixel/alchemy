	.section .rodata
	.balign 4
@ 敵 165 体
	.global Enemy_Definitions
Enemy_Definitions:
	.incbin "GAME/CHARACTER/ENEMIES.table"
