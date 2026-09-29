@ Battle_BlueRuinsBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_BlueRuinsBackdrop
Battle_BlueRuinsBackdrop:
	.incbin "GRAPHICS/BACKDROP/BLUE_RUINS.gbapal"
	.incbin "GRAPHICS/BACKDROP/BLUE_RUINS.bitmap.d7"
