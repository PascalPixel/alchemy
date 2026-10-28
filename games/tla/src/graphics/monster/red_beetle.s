	.section .rodata
	.global Monster_RedBeetleSprites
Monster_RedBeetleSprites:
	.incbin "games/tla/src/graphics/monster/red_beetle.pictures.lz"
	.byte 0x82, 0x89, 0x74	@ PACKER: left in the buffer by Monster_RedBeastSprites+0x10d1
