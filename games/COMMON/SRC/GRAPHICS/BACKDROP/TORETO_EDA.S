@ Battle_ToretoEdaBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_ToretoEdaBackdrop
Battle_ToretoEdaBackdrop:
	.incbin "GRAPHICS/BACKDROP/TORETO_EDA.gbapal"
	.incbin "GRAPHICS/BACKDROP/TORETO_EDA.bitmap.d7"
