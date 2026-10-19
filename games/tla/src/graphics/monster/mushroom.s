	.section .rodata
	.global Monster_MushroomSprites
Monster_MushroomSprites:
	.incbin "games/tla/src/graphics/monster/mushroom.frames.lz"
	.byte 0x47, 0x7d, 0x5a	@ PACKER: left in the buffer by Monster_BlueWispSprites+0xd09
