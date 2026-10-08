	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_PinkStarSheet
BattleFx_PinkStarSheet:
	.incbin "GRAPHICS/BATTLE_FX/PINK_STAR.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/PINK_STAR.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_TargetBurstImage at +0x38f */
	.byte 0x80
	.endif
