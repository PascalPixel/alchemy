@ Battle_HaidiaArashiBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_HaidiaArashiBackdrop
Battle_HaidiaArashiBackdrop:
	.incbin "GRAPHICS/BACKDROP/HAIDIA_ARASHI.gbapal"
	.incbin "GRAPHICS/BACKDROP/HAIDIA_ARASHI.bitmap.d7"
