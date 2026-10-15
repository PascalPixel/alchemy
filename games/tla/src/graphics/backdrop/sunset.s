	.section .rodata
	.global Battle_SunsetBackdrop
Battle_SunsetBackdrop:
	.incbin "games/tla/src/graphics/backdrop/sunset.gbapal"
	.incbin "games/tla/src/graphics/backdrop/sunset.bitmap.d7"
	.byte 0x3b, 0x47	@ PACKER: left in the buffer by Battle_AerialViewBackdrop+0x28a2
