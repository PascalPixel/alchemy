	.section .rodata
	.global Monster_OstrichSprites
Monster_OstrichSprites:
	.incbin "games/tla/src/graphics/monster/ostrich.pictures.lz"
	.byte 0x1e, 0x02	@ PACKER: left in the buffer by Monster_SkullMageSprites+0x2c76
