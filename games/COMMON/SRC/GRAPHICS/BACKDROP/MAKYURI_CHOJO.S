@ Battle_MakyuriChojoBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_MakyuriChojoBackdrop
Battle_MakyuriChojoBackdrop:
	.incbin "GRAPHICS/BACKDROP/MAKYURI_CHOJO.gbapal"
	.incbin "GRAPHICS/BACKDROP/MAKYURI_CHOJO.bitmap.d7"
