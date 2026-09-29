@ Battle_DuskCloudsBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_DuskCloudsBackdrop
Battle_DuskCloudsBackdrop:
	.incbin "GRAPHICS/BACKDROP/DUSK_CLOUDS.gbapal"
	.incbin "GRAPHICS/BACKDROP/DUSK_CLOUDS.bitmap.d7"
