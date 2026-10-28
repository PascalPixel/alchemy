	.section .rodata
	.global Monster_TrollSprites
Monster_TrollSprites:
	.incbin "games/tla/src/graphics/monster/troll.pictures.lz"
	.byte 0x3c, 0x86	@ PACKER: left in the buffer by Monster_GreenMageSprites+0x3222
