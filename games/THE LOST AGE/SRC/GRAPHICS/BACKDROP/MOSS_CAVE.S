@ Battle_MossCaveBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_MossCaveBackdrop
Battle_MossCaveBackdrop:
	.incbin "GRAPHICS/BACKDROP/MOSS_CAVE.gbapal"
	.incbin "GRAPHICS/BACKDROP/MOSS_CAVE.bitmap.d7"
