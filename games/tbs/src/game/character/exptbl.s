	.section .rodata
@ 属性の組
	.global Character_ElementGroupTable
Character_ElementGroupTable:
	.byte 0, 0, 1, 1, 0, 0, 1, 1
	.balign 4
@ レベルごとの経験値
	.global Character_LevelExpTable
Character_LevelExpTable:
	.incbin "game/character/lvexp_0.table"
	.incbin "game/character/lvexp_1.table"
	.incbin "game/character/lvexp_2.table"
	.incbin "game/character/lvexp_3.table"
	.incbin "game/character/lvexp_4.table"
	.incbin "game/character/lvexp_5.table"
	.incbin "game/character/lvexp_6.table"
	.incbin "game/character/lvexp_7.table"
