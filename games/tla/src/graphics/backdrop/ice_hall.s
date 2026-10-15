	.section .rodata
	.global Battle_IceHallBackdrop
Battle_IceHallBackdrop:
	.incbin "games/tla/src/graphics/backdrop/ice_hall.gbapal"
	.incbin "games/tla/src/graphics/backdrop/ice_hall.bitmap.d7"
	.byte 0xe8, 0xaa	@ PACKER: left in the buffer by Battle_StormySeaBackdrop+0x444e
