@ Battle_LavaCaveBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_LavaCaveBackdrop
Battle_LavaCaveBackdrop:
	.incbin "GRAPHICS/BACKDROP/LAVA_CAVE.gbapal"
	.incbin "GRAPHICS/BACKDROP/LAVA_CAVE.bitmap.d7"
