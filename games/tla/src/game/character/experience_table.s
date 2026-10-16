	.section .rodata
	.balign 4
@ Each character's element group, as The Broken Seal has it: 0 for the
@ Venus and Mars Adepts, 1 for the Jupiter and Mercury ones.
	.global gCharacter_ElementGroupTable
gCharacter_ElementGroupTable:
	.byte 0, 0, 1, 1, 0, 0, 1, 1
@ Six flags Func_080ae410 copies into game flags 0x820 to 0x825.
	.global Data_080b1284
Data_080b1284:
	.2byte 0x941, 0x951, 0x8b3, 0x8d1, 0x81e, 0x868
@ The percent chance of each of seven Djinn of each element that
@ Func_080ae868 gives to Djinn_AddToLeastLoadedOwner, one row per element.
	.global Data_080b1290
Data_080b1290:
	.2byte 100, 70, 70, 30, 40, 50, 20
	.2byte 100, 50, 70, 70, 30, 30, 70
	.2byte 70, 50, 30, 60, 70, 40, 70
	.2byte 50, 50, 70, 50, 20, 70, 70
	.balign 4
@ Experience per level.
	.global gOwner_ExperienceThresholds
gOwner_ExperienceThresholds:
	.incbin "games/tla/src/game/character/level_experience_0.table"
	.incbin "games/tla/src/game/character/level_experience_1.table"
	.incbin "games/tla/src/game/character/level_experience_2.table"
	.incbin "games/tla/src/game/character/level_experience_3.table"
	.incbin "games/tla/src/game/character/level_experience_4.table"
	.incbin "games/tla/src/game/character/level_experience_5.table"
	.incbin "games/tla/src/game/character/level_experience_6.table"
	.incbin "games/tla/src/game/character/level_experience_7.table"
