	.section .rodata
	.global Ending_CliffSunsetPicture
Ending_CliffSunsetPicture:
	.incbin "games/tla/src/graphics/ending/cliff_sunset.gbapal"
	.incbin "games/tla/src/graphics/ending/cliff_sunset.bitmap.d7"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
