	.section .rodata
	.balign 4
@ レベルごとの経験値
	.global Owner_ExperienceThresholds
Owner_ExperienceThresholds:
	.incbin "game/character/lvexp_0.table"
	.incbin "game/character/lvexp_1.table"
	.incbin "game/character/lvexp_2.table"
	.incbin "game/character/lvexp_3.table"
	.incbin "game/character/lvexp_4.table"
	.incbin "game/character/lvexp_5.table"
	.incbin "game/character/lvexp_6.table"
	.incbin "game/character/lvexp_7.table"
