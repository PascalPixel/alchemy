@ Battle_BeachBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_BeachBackdrop
Battle_BeachBackdrop:
	.incbin "GRAPHICS/BACKDROP/BEACH.gbapal"
	.incbin "GRAPHICS/BACKDROP/BEACH.bitmap.d7"
