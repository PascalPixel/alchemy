@ Battle_GreenRuinsBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_GreenRuinsBackdrop
Battle_GreenRuinsBackdrop:
	.incbin "GRAPHICS/BACKDROP/GREEN_RUINS.gbapal"
	.incbin "GRAPHICS/BACKDROP/GREEN_RUINS.bitmap.d7"
