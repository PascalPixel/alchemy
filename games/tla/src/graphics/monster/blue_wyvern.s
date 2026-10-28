	.section .rodata
	.global Monster_BlueWyvernSprites
Monster_BlueWyvernSprites:
	.incbin "games/tla/src/graphics/monster/blue_wyvern.pictures.lz"
	.byte 0x12, 0x2a, 0x02	@ PACKER: left in the buffer by Monster_OstrichSprites+0x1ef9
