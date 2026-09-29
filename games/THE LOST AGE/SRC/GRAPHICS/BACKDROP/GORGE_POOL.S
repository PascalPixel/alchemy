@ Battle_GorgePoolBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_GorgePoolBackdrop
Battle_GorgePoolBackdrop:
	.incbin "GRAPHICS/BACKDROP/GORGE_POOL.gbapal"
	.incbin "GRAPHICS/BACKDROP/GORGE_POOL.bitmap.d7"
