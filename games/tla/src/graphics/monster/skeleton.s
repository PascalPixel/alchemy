	.section .rodata
	.global Monster_SkeletonSprites
Monster_SkeletonSprites:
	.incbin "games/tla/src/graphics/monster/skeleton.frames.lz"
	.byte 0xe6, 0x60, 0x43	@ PACKER: left in the buffer by Monster_WingedDragonSprites+0x2a39
