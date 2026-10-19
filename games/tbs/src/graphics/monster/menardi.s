	.section .rodata
	.global Monster_MenardiSprites
Monster_MenardiSprites:
	.incbin "games/tbs/src/graphics/monster/menardi.frames.lz"
	.byte 0x52	@ PACKER: left in the buffer by Monster_SaturosSprites+0x3da7
