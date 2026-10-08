	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_ShieldSheet
BattleFx_ShieldSheet:
	.incbin "GRAPHICS/BATTLE_FX/SHIELD.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SHIELD.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x45, 0xb1, 0x12	@ PACKER: left in the buffer by BattleFx_FireStreakSheet+0xcad
	.endif
