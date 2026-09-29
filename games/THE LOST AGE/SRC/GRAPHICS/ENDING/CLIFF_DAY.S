@ Ending_CliffDayPicture: a 512x120 panorama: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Ending_CliffDayPicture
Ending_CliffDayPicture:
	.incbin "GRAPHICS/ENDING/CLIFF_DAY.gbapal"
	.incbin "GRAPHICS/ENDING/CLIFF_DAY.bitmap.d7"
