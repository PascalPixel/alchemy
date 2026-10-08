	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_BlueArcSheetB
BattleFx_BlueArcSheetB:
	.incbin "GRAPHICS/BATTLE_FX/BLUE_ARC_B.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BLUE_ARC_B.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_EmberStreakSheet at +0xace */
	.byte 0xaa, 0x30
	.endif
