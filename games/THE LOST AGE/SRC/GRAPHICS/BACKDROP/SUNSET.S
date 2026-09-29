@ Battle_SunsetBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_SunsetBackdrop
Battle_SunsetBackdrop:
	.incbin "GRAPHICS/BACKDROP/SUNSET.gbapal"
	.incbin "GRAPHICS/BACKDROP/SUNSET.bitmap.d7"
