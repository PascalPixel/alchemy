	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_BlueArcSheetA
BattleFx_BlueArcSheetA:
	.incbin "GRAPHICS/BATTLE_FX/BLUE_ARC_A.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BLUE_ARC_A.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_EmberStreakSheet at +0x7d5 */
	.byte 0x15, 0xc0, 0x04
	.endif
