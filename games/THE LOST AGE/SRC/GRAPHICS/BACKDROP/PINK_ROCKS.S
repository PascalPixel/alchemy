@ Battle_PinkRocksBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_PinkRocksBackdrop
Battle_PinkRocksBackdrop:
	.incbin "GRAPHICS/BACKDROP/PINK_ROCKS.gbapal"
	.incbin "GRAPHICS/BACKDROP/PINK_ROCKS.bitmap.d7"
