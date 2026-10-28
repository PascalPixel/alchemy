	.section .rodata
	.global Monster_GreenOgreSprites
Monster_GreenOgreSprites:
	.incbin "games/tla/src/graphics/monster/green_ogre.pictures.lz"
	.byte 0x90, 0x49, 0x50	@ PACKER: left in the buffer by Monster_TreantSprites+0x2b79
