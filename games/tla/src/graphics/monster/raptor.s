	.section .rodata
	.global Monster_RaptorSprites
Monster_RaptorSprites:
	.incbin "games/tla/src/graphics/monster/raptor.pictures.lz"
	.byte 0xaa, 0x28	@ PACKER: left in the buffer by Monster_MummySprites+0x1aee
