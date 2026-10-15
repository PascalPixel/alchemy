	.section .rodata
	.global Monster_ToadSprites
Monster_ToadSprites:
	.incbin "games/tla/src/graphics/monster/toad.frames.lz"
	.byte 0x06	@ PACKER: left in the buffer by Monster_VultureSprites+0x1047
