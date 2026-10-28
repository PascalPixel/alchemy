	.section .rodata
	.global Monster_BatSprites
Monster_BatSprites:
	.incbin "games/tla/src/graphics/monster/bat.pictures.lz"
	.byte 0x3c, 0xcc, 0xb9	@ PACKER: left in the buffer by Monster_RedWormSprites+0x7dd
