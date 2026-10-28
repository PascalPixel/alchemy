	.section .rodata
	.global Monster_MenardiSprites
Monster_MenardiSprites:
	.incbin "games/tla/src/graphics/monster/menardi.pictures.lz"
	.byte 0x52	@ PACKER: left in the buffer by Monster_SaturosSprites+0x3da7
