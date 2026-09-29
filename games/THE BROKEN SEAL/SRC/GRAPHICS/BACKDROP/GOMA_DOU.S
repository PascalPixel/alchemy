@ Battle_GomaDouBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_GomaDouBackdrop
Battle_GomaDouBackdrop:
	.incbin "GRAPHICS/BACKDROP/GOMA_DOU.gbapal"
	.incbin "GRAPHICS/BACKDROP/GOMA_DOU.bitmap.d7"
