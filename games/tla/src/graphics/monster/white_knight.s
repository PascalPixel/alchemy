	.section .rodata
	.global Monster_WhiteKnightSprites
Monster_WhiteKnightSprites:
	.incbin "games/tla/src/graphics/monster/white_knight.frames.lz"
	.byte 0x8d, 0x1e, 0x36	@ PACKER: left in the buffer by Monster_RedBeastSprites+0x4b49
