@ Battle_DesertBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_DesertBackdrop
Battle_DesertBackdrop:
	.incbin "GRAPHICS/BACKDROP/DESERT.gbapal"
	.incbin "GRAPHICS/BACKDROP/DESERT.bitmap.d7"
