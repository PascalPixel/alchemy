	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_FireStreakSheet
BattleFx_FireStreakSheet:
	.incbin "GRAPHICS/BATTLE_FX/FIRE_STREAK.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FIRE_STREAK.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_WhirlwindSheet at +0x15c2 */
	.byte 0x00, 0x48
	.endif
