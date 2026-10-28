	.section .rodata
	.global Monster_DragonRightSprites
Monster_DragonRightSprites:
	.incbin "games/tla/src/graphics/monster/dragon_right.pictures.lz"
	.byte 0x4c	@ PACKER: left in the buffer by Monster_DragonLeftSprites+0x42c7
