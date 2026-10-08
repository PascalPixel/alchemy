	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_PinkStarSheet
BattleFx_PinkStarSheet:
	.incbin "GRAPHICS/BATTLE_FX/PINK_STAR.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/PINK_STAR.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x80	@ PACKER: left in the buffer by BattleFx_TargetBurstImage+0x38f
	.endif
