	.section .rodata
	.global Monster_StorkSprites
Monster_StorkSprites:
	.incbin "games/tla/src/graphics/monster/stork.frames.lz"
	.byte 0xd6, 0x91	@ PACKER: left in the buffer by Monster_SwordsmanSprites+0x17e2
