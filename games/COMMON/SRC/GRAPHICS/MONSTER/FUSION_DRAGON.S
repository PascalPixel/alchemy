	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_FusionDragonSprites
Monster_FusionDragonSprites:
	.incbin "GRAPHICS/MONSTER/FUSION_DRAGON.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by MONSTER/FIRE_DRAGON.o at +0x47f3 */
	.byte 0x70
	.endif
