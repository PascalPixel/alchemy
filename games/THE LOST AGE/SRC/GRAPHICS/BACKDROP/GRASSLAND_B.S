@ Battle_GrasslandBBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_GrasslandBBackdrop
Battle_GrasslandBBackdrop:
	.incbin "GRAPHICS/BACKDROP/GRASSLAND_B.gbapal"
	.incbin "GRAPHICS/BACKDROP/GRASSLAND_B.bitmap.d7"
