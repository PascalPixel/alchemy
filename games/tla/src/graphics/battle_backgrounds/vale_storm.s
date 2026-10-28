	.section .rodata
	.global Battle_ValeStormBackdrop
Battle_ValeStormBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/vale_storm.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/vale_storm.bitmap.d7"
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
