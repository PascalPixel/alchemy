	.section .rodata
	.global Battle_SuhallaGateBackdrop
Battle_SuhallaGateBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/suhalla_gate.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/suhalla_gate.bitmap.d7"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
