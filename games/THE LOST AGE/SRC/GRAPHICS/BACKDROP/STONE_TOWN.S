@ Battle_StoneTownBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_StoneTownBackdrop
Battle_StoneTownBackdrop:
	.incbin "GRAPHICS/BACKDROP/STONE_TOWN.gbapal"
	.incbin "GRAPHICS/BACKDROP/STONE_TOWN.bitmap.d7"
