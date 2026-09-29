@ Battle_PinkTotemBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_PinkTotemBackdrop
Battle_PinkTotemBackdrop:
	.incbin "GRAPHICS/BACKDROP/PINK_TOTEM.gbapal"
	.incbin "GRAPHICS/BACKDROP/PINK_TOTEM.bitmap.d7"
