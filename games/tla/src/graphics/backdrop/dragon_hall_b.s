	.section .rodata
	.global Battle_DragonHallBBackdrop
Battle_DragonHallBBackdrop:
	.incbin "games/tla/src/graphics/backdrop/dragon_hall_b.gbapal"
	.incbin "games/tla/src/graphics/backdrop/dragon_hall_b.bitmap.d7"
	.byte 0x44, 0xf4	@ PACKER: left in the buffer by Battle_DragonHallABackdrop+0x3c4e
