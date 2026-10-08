	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_VioletStreakSheet
BattleFx_VioletStreakSheet:
	.incbin "GRAPHICS/BATTLE_FX/VIOLET_STREAK.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/VIOLET_STREAK.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_SpiralSheet at +0xc8b */
	.byte 0x5b
	.endif
