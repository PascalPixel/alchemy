	.section .rodata
	.global Monster_SwordImpSprites
Monster_SwordImpSprites:
	.incbin "games/tla/src/graphics/monster/sword_imp.pictures.lz"
	.byte 0xa1	@ PACKER: left in the buffer by Monster_ApeSprites+0xc23
