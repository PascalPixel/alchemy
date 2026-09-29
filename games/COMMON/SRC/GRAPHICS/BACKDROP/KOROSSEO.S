@ Battle_KorosseoBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_KorosseoBackdrop
Battle_KorosseoBackdrop:
	.incbin "GRAPHICS/BACKDROP/KOROSSEO.gbapal"
	.incbin "GRAPHICS/BACKDROP/KOROSSEO.bitmap.d7"
