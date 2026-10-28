	.section .rodata
	.global Monster_UrchinSprites
Monster_UrchinSprites:
	.incbin "games/tla/src/graphics/monster/urchin.pictures.lz"
	.byte 0x32, 0x91	@ PACKER: left in the buffer by Monster_JackalSprites+0x1712
