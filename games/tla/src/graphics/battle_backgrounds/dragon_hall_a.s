	.section .rodata
	.global Battle_DragonHallABackdrop
Battle_DragonHallABackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/dragon_hall_a.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/dragon_hall_a.bitmap.d7"
	.byte 0x05, 0x41	@ PACKER: left in the buffer by Battle_BeastWallBackdrop+0x3d8e
