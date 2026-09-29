@ Battle_ToretoHeyaBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_ToretoHeyaBackdrop
Battle_ToretoHeyaBackdrop:
	.incbin "GRAPHICS/BACKDROP/TORETO_HEYA.gbapal"
	.incbin "GRAPHICS/BACKDROP/TORETO_HEYA.bitmap.d7"
