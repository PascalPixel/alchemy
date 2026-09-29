@ Battle_StoneCorridorBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_StoneCorridorBackdrop
Battle_StoneCorridorBackdrop:
	.incbin "GRAPHICS/BACKDROP/STONE_CORRIDOR.gbapal"
	.incbin "GRAPHICS/BACKDROP/STONE_CORRIDOR.bitmap.d7"
