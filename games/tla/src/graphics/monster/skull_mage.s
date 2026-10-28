	.section .rodata
	.global Monster_SkullMageSprites
Monster_SkullMageSprites:
	.incbin "games/tla/src/graphics/monster/skull_mage.pictures.lz"
	.byte 0x05, 0xf5, 0x36	@ PACKER: left in the buffer by Monster_RedBeastSprites+0x3141
