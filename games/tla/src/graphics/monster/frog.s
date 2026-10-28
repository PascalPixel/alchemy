	.section .rodata
	.global Monster_FrogSprites
Monster_FrogSprites:
	.incbin "games/tla/src/graphics/monster/frog.pictures.lz"
	.byte 0xb3	@ PACKER: left in the buffer by Monster_SmallBirdSprites+0xc1b
