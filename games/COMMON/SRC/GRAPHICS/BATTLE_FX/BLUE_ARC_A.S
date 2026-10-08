	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_BlueArcSheetA
BattleFx_BlueArcSheetA:
	.incbin "GRAPHICS/BATTLE_FX/BLUE_ARC_A.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BLUE_ARC_A.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x15, 0xc0, 0x04	@ PACKER: left in the buffer by BattleFx_EmberStreakSheet+0x7d5
	.endif
