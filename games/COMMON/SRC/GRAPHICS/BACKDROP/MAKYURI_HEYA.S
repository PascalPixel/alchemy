@ Battle_MakyuriHeyaBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_MakyuriHeyaBackdrop
Battle_MakyuriHeyaBackdrop:
	.incbin "GRAPHICS/BACKDROP/MAKYURI_HEYA.gbapal"
	.incbin "GRAPHICS/BACKDROP/MAKYURI_HEYA.bitmap.d7"
