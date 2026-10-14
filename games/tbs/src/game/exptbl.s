	.section .rodata
@ 属性の組
	.global Character_ElementGroupTable
Character_ElementGroupTable:
	.byte 0, 0, 1, 1, 0, 0, 1, 1
	.balign 4
@ レベルごとの経験値
	.global Character_LevelExpTable
Character_LevelExpTable:
	.incbin "games/tbs/src/game/lvexp_0.table"
	.incbin "games/tbs/src/game/lvexp_1.table"
	.incbin "games/tbs/src/game/lvexp_2.table"
	.incbin "games/tbs/src/game/lvexp_3.table"
	.incbin "games/tbs/src/game/lvexp_4.table"
	.incbin "games/tbs/src/game/lvexp_5.table"
	.incbin "games/tbs/src/game/lvexp_6.table"
	.incbin "games/tbs/src/game/lvexp_7.table"
