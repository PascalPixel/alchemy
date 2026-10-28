	.section .rodata
	.global Monster_AquaBeastSprites
Monster_AquaBeastSprites:
	.incbin "games/tla/src/graphics/monster/aqua_beast.pictures.lz"
	.byte 0xba, 0x77, 0xc9	@ PACKER: left in the buffer by Monster_SalamanderSprites+0xce9
