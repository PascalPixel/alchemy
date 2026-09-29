@ Battle_FireCavePoolBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_FireCavePoolBackdrop
Battle_FireCavePoolBackdrop:
	.incbin "GRAPHICS/BACKDROP/FIRE_CAVE_POOL.gbapal"
	.incbin "GRAPHICS/BACKDROP/FIRE_CAVE_POOL.bitmap.d7"
