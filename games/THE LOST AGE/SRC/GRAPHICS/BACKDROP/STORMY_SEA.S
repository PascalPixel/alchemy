@ Battle_StormySeaBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_StormySeaBackdrop
Battle_StormySeaBackdrop:
	.incbin "GRAPHICS/BACKDROP/STORMY_SEA.gbapal"
	.incbin "GRAPHICS/BACKDROP/STORMY_SEA.bitmap.d7"
