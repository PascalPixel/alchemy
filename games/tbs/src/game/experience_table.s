	.section .rodata
@ Each character's element group.
	.global gCharacter_ElementGroupTable
gCharacter_ElementGroupTable:
	.byte 0, 0, 1, 1, 0, 0, 1, 1
	.balign 4
@ Experience required at each level.
	.global gCharacter_LevelExpTable
gCharacter_LevelExpTable:
	.incbin "games/tbs/src/game/level_experience_0.table"
	.incbin "games/tbs/src/game/level_experience_1.table"
	.incbin "games/tbs/src/game/level_experience_2.table"
	.incbin "games/tbs/src/game/level_experience_3.table"
	.incbin "games/tbs/src/game/level_experience_4.table"
	.incbin "games/tbs/src/game/level_experience_5.table"
	.incbin "games/tbs/src/game/level_experience_6.table"
	.incbin "games/tbs/src/game/level_experience_7.table"
