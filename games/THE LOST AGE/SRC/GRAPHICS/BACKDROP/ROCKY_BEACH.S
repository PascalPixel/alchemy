@ Battle_RockyBeachBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_RockyBeachBackdrop
Battle_RockyBeachBackdrop:
	.incbin "GRAPHICS/BACKDROP/ROCKY_BEACH.gbapal"
	.incbin "GRAPHICS/BACKDROP/ROCKY_BEACH.bitmap.d7"
