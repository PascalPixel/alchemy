@ Battle_AngelHallBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_AngelHallBackdrop
Battle_AngelHallBackdrop:
	.incbin "GRAPHICS/BACKDROP/ANGEL_HALL.gbapal"
	.incbin "GRAPHICS/BACKDROP/ANGEL_HALL.bitmap.d7"
