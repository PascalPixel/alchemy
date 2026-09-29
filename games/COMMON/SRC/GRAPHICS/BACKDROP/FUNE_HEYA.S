@ Battle_FuneHeyaBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_FuneHeyaBackdrop
Battle_FuneHeyaBackdrop:
	.incbin "GRAPHICS/BACKDROP/FUNE_HEYA.gbapal"
	.incbin "GRAPHICS/BACKDROP/FUNE_HEYA.bitmap.d7"
