	.section .rodata
	.global Monster_OrcSprites
Monster_OrcSprites:
	.incbin "games/tla/src/graphics/monster/orc.pictures.lz"
	.byte 0xfe, 0x7b	@ PACKER: left in the buffer by Monster_GriffinSprites+0x260e
