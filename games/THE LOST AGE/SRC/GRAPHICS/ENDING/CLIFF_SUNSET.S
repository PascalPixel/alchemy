@ Ending_CliffSunsetPicture: a 512x120 panorama: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Ending_CliffSunsetPicture
Ending_CliffSunsetPicture:
	.incbin "GRAPHICS/ENDING/CLIFF_SUNSET.gbapal"
	.incbin "GRAPHICS/ENDING/CLIFF_SUNSET.bitmap.d7"
