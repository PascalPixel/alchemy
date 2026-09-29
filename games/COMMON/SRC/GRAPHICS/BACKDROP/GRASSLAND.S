@ Battle_GrasslandBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_GrasslandBackdrop
Battle_GrasslandBackdrop:
	.incbin "GRAPHICS/BACKDROP/GRASSLAND.gbapal"
	.incbin "GRAPHICS/BACKDROP/GRASSLAND.bitmap.d7"
