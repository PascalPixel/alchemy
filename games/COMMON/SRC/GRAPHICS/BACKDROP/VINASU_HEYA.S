@ Battle_VinasuHeyaBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_VinasuHeyaBackdrop
Battle_VinasuHeyaBackdrop:
	.incbin "GRAPHICS/BACKDROP/VINASU_HEYA.gbapal"
	.incbin "GRAPHICS/BACKDROP/VINASU_HEYA.bitmap.d7"
