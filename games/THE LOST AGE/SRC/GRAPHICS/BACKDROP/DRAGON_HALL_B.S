@ Battle_DragonHallBBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_DragonHallBBackdrop
Battle_DragonHallBBackdrop:
	.incbin "GRAPHICS/BACKDROP/DRAGON_HALL_B.gbapal"
	.incbin "GRAPHICS/BACKDROP/DRAGON_HALL_B.bitmap.d7"
