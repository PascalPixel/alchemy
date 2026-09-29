@ Battle_LavaTempleBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_LavaTempleBackdrop
Battle_LavaTempleBackdrop:
	.incbin "GRAPHICS/BACKDROP/LAVA_TEMPLE.gbapal"
	.incbin "GRAPHICS/BACKDROP/LAVA_TEMPLE.bitmap.d7"
