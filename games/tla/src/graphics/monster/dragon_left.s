	.section .rodata
	.global Monster_DragonLeftSprites
Monster_DragonLeftSprites:
	.incbin "games/tla/src/graphics/monster/dragon_left.pictures.lz"
	.byte 0xc0	@ PACKER: left in the buffer by Monster_HydraSprites+0x4623
