	.section .rodata
	.global Monster_SmallBirdSprites
Monster_SmallBirdSprites:
	.incbin "games/tla/src/graphics/monster/small_bird.frames.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
