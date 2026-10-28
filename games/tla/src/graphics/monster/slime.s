	.section .rodata
	.global Monster_SlimeSprites
Monster_SlimeSprites:
	.incbin "games/tla/src/graphics/monster/slime.pictures.lz"
	.byte 0x40, 0x2c	@ PACKER: left in the buffer by Monster_KrakenSprites+0x36c6
