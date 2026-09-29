@ Battle_GoldRocksBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_GoldRocksBackdrop
Battle_GoldRocksBackdrop:
	.incbin "GRAPHICS/BACKDROP/GOLD_ROCKS.gbapal"
	.incbin "GRAPHICS/BACKDROP/GOLD_ROCKS.bitmap.d7"
