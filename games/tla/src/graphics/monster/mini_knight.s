	.section .rodata
	.global Monster_MiniKnightSprites
Monster_MiniKnightSprites:
	.incbin "games/tla/src/graphics/monster/mini_knight.frames.lz"
	.byte 0x05	@ PACKER: left in the buffer by Monster_BrawlerSprites+0x13c3
