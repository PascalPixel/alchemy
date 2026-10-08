	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_FireStreakSheet
BattleFx_FireStreakSheet:
	.incbin "GRAPHICS/BATTLE_FX/FIRE_STREAK.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FIRE_STREAK.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x00, 0x48	@ PACKER: left in the buffer by BattleFx_WhirlwindSheet+0x15c2
	.endif
