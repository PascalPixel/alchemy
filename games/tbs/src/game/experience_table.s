	.section .rodata
@ 属性の組
	.global Character_ElementGroupTable
Character_ElementGroupTable:
	.byte 0, 0, 1, 1, 0, 0, 1, 1
	.balign 4
@ レベルごとの経験値
	.global Character_LevelExpTable
Character_LevelExpTable:
	.incbin "games/tbs/src/game/level_experience_0.table"
	.incbin "games/tbs/src/game/level_experience_1.table"
	.incbin "games/tbs/src/game/level_experience_2.table"
	.incbin "games/tbs/src/game/level_experience_3.table"
	.incbin "games/tbs/src/game/level_experience_4.table"
	.incbin "games/tbs/src/game/level_experience_5.table"
	.incbin "games/tbs/src/game/level_experience_6.table"
	.incbin "games/tbs/src/game/level_experience_7.table"
