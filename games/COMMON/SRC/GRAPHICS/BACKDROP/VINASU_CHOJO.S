@ Battle_VinasuChojoBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_VinasuChojoBackdrop
Battle_VinasuChojoBackdrop:
	.incbin "GRAPHICS/BACKDROP/VINASU_CHOJO.gbapal"
	.incbin "GRAPHICS/BACKDROP/VINASU_CHOJO.bitmap.d7"
