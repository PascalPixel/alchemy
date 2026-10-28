	.section .rodata
	.global Monster_JackalSprites
Monster_JackalSprites:
	.incbin "games/tla/src/graphics/monster/jackal.pictures.lz"
	.byte 0xe1, 0xc9, 0x58	@ PACKER: left in the buffer by Monster_HydraSprites+0x2729
