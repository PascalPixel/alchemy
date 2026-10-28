	.section .rodata
	.global Monster_PinkWormSprites
Monster_PinkWormSprites:
	.incbin "games/tla/src/graphics/monster/pink_worm.pictures.lz"
	.byte 0x09, 0x25, 0x74	@ PACKER: left in the buffer by Monster_RedBeetleSprites+0x1069
