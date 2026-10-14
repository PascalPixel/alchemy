	.section .rodata
	.balign 4
@ レベルごとの経験値
	.global Owner_ExperienceThresholds
Owner_ExperienceThresholds:
	.incbin "games/tla/src/game/character/lvexp_0.table"
	.incbin "games/tla/src/game/character/lvexp_1.table"
	.incbin "games/tla/src/game/character/lvexp_2.table"
	.incbin "games/tla/src/game/character/lvexp_3.table"
	.incbin "games/tla/src/game/character/lvexp_4.table"
	.incbin "games/tla/src/game/character/lvexp_5.table"
	.incbin "games/tla/src/game/character/lvexp_6.table"
	.incbin "games/tla/src/game/character/lvexp_7.table"
