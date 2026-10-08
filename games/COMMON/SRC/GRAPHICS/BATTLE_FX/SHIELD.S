	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_ShieldSheet
BattleFx_ShieldSheet:
	.incbin "GRAPHICS/BATTLE_FX/SHIELD.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SHIELD.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_FireStreakSheet at +0xcad */
	.byte 0x45, 0xb1, 0x12
	.endif
