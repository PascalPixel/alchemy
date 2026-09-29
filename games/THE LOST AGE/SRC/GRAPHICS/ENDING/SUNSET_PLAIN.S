@ Ending_SunsetPlainPicture: a 512x120 panorama: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Ending_SunsetPlainPicture
Ending_SunsetPlainPicture:
	.incbin "GRAPHICS/ENDING/SUNSET_PLAIN.gbapal"
	.incbin "GRAPHICS/ENDING/SUNSET_PLAIN.bitmap.d7"
