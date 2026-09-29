@ Battle_PalmCliffBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_PalmCliffBackdrop
Battle_PalmCliffBackdrop:
	.incbin "GRAPHICS/BACKDROP/PALM_CLIFF.gbapal"
	.incbin "GRAPHICS/BACKDROP/PALM_CLIFF.bitmap.d7"
