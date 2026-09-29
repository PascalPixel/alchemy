@ Battle_BeastWallBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_BeastWallBackdrop
Battle_BeastWallBackdrop:
	.incbin "GRAPHICS/BACKDROP/BEAST_WALL.gbapal"
	.incbin "GRAPHICS/BACKDROP/BEAST_WALL.bitmap.d7"
