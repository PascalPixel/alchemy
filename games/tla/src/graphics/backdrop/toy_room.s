	.section .rodata
	.global Battle_ToyRoomBackdrop
Battle_ToyRoomBackdrop:
	.incbin "games/tla/src/graphics/backdrop/toy_room.gbapal"
	.incbin "games/tla/src/graphics/backdrop/toy_room.bitmap.d7"
	.byte 0x15, 0xdd	@ PACKER: left in the buffer by Battle_PalmCliffBackdrop+0x394a
