@ Battle_DenseForestBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_DenseForestBackdrop
Battle_DenseForestBackdrop:
	.incbin "GRAPHICS/BACKDROP/DENSE_FOREST.gbapal"
	.incbin "GRAPHICS/BACKDROP/DENSE_FOREST.bitmap.d7"
