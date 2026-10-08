	.section .rodata
@ 属性の組
	.global Character_ElementGroupTable
Character_ElementGroupTable:
	.byte 0, 0, 1, 1, 0, 0, 1, 1
	.balign 4
@ レベルごとの経験値
	.global Character_LevelExpTable
Character_LevelExpTable:
	.incbin "GAME/CHARACTER/LVEXP_0.table"
	.incbin "GAME/CHARACTER/LVEXP_1.table"
	.incbin "GAME/CHARACTER/LVEXP_2.table"
	.incbin "GAME/CHARACTER/LVEXP_3.table"
	.incbin "GAME/CHARACTER/LVEXP_4.table"
	.incbin "GAME/CHARACTER/LVEXP_5.table"
	.incbin "GAME/CHARACTER/LVEXP_6.table"
	.incbin "GAME/CHARACTER/LVEXP_7.table"
