@ Battle_PurpleCaveBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_PurpleCaveBackdrop
Battle_PurpleCaveBackdrop:
	.incbin "GRAPHICS/BACKDROP/PURPLE_CAVE.gbapal"
	.incbin "GRAPHICS/BACKDROP/PURPLE_CAVE.bitmap.d7"
