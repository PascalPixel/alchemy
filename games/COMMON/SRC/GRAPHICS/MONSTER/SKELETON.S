	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_SkeletonSprites
Monster_SkeletonSprites:
	.incbin "GRAPHICS/MONSTER/SKELETON.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Monster_WingedDragonSprites at +0x2a39 */
	.byte 0xe6, 0x60, 0x43
	.endif
