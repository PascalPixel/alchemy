	.section .rodata
	.global Monster_MermanSprites
Monster_MermanSprites:
	.incbin "games/tla/src/graphics/monster/merman.frames.lz"
	.byte 0x1e, 0x04, 0x98	@ PACKER: left in the buffer by Monster_ScorpionSprites+0x2d61
