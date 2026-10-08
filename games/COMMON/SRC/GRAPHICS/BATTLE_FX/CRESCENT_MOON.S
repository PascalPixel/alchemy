	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_CrescentMoonSheet
BattleFx_CrescentMoonSheet:
	.incbin "GRAPHICS/BATTLE_FX/CRESCENT_MOON.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/CRESCENT_MOON.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_VioletStreakSheet at +0x4cf */
	.byte 0x2b
	.endif
