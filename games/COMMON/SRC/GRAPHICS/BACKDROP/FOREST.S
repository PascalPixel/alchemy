@ Battle_ForestBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_ForestBackdrop
Battle_ForestBackdrop:
	.incbin "GRAPHICS/BACKDROP/FOREST.gbapal"
	.incbin "GRAPHICS/BACKDROP/FOREST.bitmap.d7"
