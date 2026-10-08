	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_FusionDragonSprites
Monster_FusionDragonSprites:
	.incbin "GRAPHICS/MONSTER/FUSION_DRAGON.frames.lz"
	.ifeq GAME_TLA
	.byte 0x70	@ PACKER: left in the buffer by Monster_FireDragonSprites+0x47f3
	.endif
