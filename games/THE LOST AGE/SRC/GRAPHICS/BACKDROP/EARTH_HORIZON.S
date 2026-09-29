@ Battle_EarthHorizonBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_EarthHorizonBackdrop
Battle_EarthHorizonBackdrop:
	.incbin "GRAPHICS/BACKDROP/EARTH_HORIZON.gbapal"
	.incbin "GRAPHICS/BACKDROP/EARTH_HORIZON.bitmap.d7"
