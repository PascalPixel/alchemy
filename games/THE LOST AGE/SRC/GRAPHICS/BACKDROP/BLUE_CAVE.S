@ Battle_BlueCaveBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_BlueCaveBackdrop
Battle_BlueCaveBackdrop:
	.incbin "GRAPHICS/BACKDROP/BLUE_CAVE.gbapal"
	.incbin "GRAPHICS/BACKDROP/BLUE_CAVE.bitmap.d7"
