	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_FlameSkullSprites
Monster_FlameSkullSprites:
	.incbin "GRAPHICS/MONSTER/FLAME_SKULL.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Monster_SkeletonSprites at +0xbea */
	.byte 0xc1, 0xb3
	.endif
