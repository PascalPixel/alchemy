	.section .rodata
	.global Battle_PurpleMoonBackdrop
Battle_PurpleMoonBackdrop:
	.incbin "games/tla/src/graphics/backdrop/purple_moon.gbapal"
	.incbin "games/tla/src/graphics/backdrop/purple_moon.bitmap.d7"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
