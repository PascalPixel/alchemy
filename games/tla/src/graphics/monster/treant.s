	.section .rodata
	.global Monster_TreantSprites
Monster_TreantSprites:
	.incbin "games/tla/src/graphics/monster/treant.frames.lz"
	.byte 0xe0	@ PACKER: left in the buffer by Monster_DarkKnightSprites+0x300f
