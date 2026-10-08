	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_FlameSkullSprites
Monster_FlameSkullSprites:
	.incbin "GRAPHICS/MONSTER/FLAME_SKULL.frames.lz"
	.ifeq GAME_TLA
	.byte 0xc1, 0xb3	@ PACKER: left in the buffer by Monster_SkeletonSprites+0xbea
	.endif
