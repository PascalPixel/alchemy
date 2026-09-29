@ Battle_RedMesaBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_RedMesaBackdrop
Battle_RedMesaBackdrop:
	.incbin "GRAPHICS/BACKDROP/RED_MESA.gbapal"
	.incbin "GRAPHICS/BACKDROP/RED_MESA.bitmap.d7"
