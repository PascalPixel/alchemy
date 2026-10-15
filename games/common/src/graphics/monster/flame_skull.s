	.section .rodata
	.global Monster_FlameSkullSprites
Monster_FlameSkullSprites:
	.incbin "games/common/src/graphics/monster/flame_skull.frames.lz"
	.byte 0xc1, 0xb3	@ PACKER: left in the buffer by Monster_SkeletonSprites+0xbea
