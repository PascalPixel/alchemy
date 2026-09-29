@ Battle_ArrowBeachBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_ArrowBeachBackdrop
Battle_ArrowBeachBackdrop:
	.incbin "GRAPHICS/BACKDROP/ARROW_BEACH.gbapal"
	.incbin "GRAPHICS/BACKDROP/ARROW_BEACH.bitmap.d7"
