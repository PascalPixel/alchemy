@ Battle_IceHallBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_IceHallBackdrop
Battle_IceHallBackdrop:
	.incbin "GRAPHICS/BACKDROP/ICE_HALL.gbapal"
	.incbin "GRAPHICS/BACKDROP/ICE_HALL.bitmap.d7"
