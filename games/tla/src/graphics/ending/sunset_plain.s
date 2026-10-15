	.section .rodata
	.global Ending_SunsetPlainPicture
Ending_SunsetPlainPicture:
	.incbin "games/tla/src/graphics/ending/sunset_plain.gbapal"
	.incbin "games/tla/src/graphics/ending/sunset_plain.bitmap.d7"
	.byte 0x5e, 0x21	@ PACKER: left in the buffer by Ending_ReunionPicture+0x5e52
