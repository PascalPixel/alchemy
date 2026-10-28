	.section .rodata
	.global Monster_FlySprites
Monster_FlySprites:
	.incbin "games/tla/src/graphics/monster/fly.pictures.lz"
	.byte 0x13	@ PACKER: left in the buffer by Monster_WalkingPlantSprites+0x773
