@ Battle_PurpleMoonBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_PurpleMoonBackdrop
Battle_PurpleMoonBackdrop:
	.incbin "GRAPHICS/BACKDROP/PURPLE_MOON.gbapal"
	.incbin "GRAPHICS/BACKDROP/PURPLE_MOON.bitmap.d7"
