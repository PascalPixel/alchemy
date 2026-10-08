	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_SkeletonSprites
Monster_SkeletonSprites:
	.incbin "GRAPHICS/MONSTER/SKELETON.frames.lz"
	.ifeq GAME_TLA
	.byte 0xe6, 0x60, 0x43	@ PACKER: left in the buffer by Monster_WingedDragonSprites+0x2a39
	.endif
