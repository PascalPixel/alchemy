	.section .rodata
	.global Monster_LarvaSprites
Monster_LarvaSprites:
	.incbin "games/tla/src/graphics/monster/larva.frames.lz"
	.byte 0x15, 0x6f	@ PACKER: left in the buffer by Monster_HydraSprites+0xf9a
