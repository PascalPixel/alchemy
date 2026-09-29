@ Battle_DragonHallABackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_DragonHallABackdrop
Battle_DragonHallABackdrop:
	.incbin "GRAPHICS/BACKDROP/DRAGON_HALL_A.gbapal"
	.incbin "GRAPHICS/BACKDROP/DRAGON_HALL_A.bitmap.d7"
