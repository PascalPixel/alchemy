	.section .rodata
	.global Monster_HydraSprites
Monster_HydraSprites:
	.incbin "games/tla/src/graphics/monster/hydra.pictures.lz"
	.byte 0x38, 0x41	@ PACKER: left in the buffer by Monster_WhiteKnightSprites+0x49a6
