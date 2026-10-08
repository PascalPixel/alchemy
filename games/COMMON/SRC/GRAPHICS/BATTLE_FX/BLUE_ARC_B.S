	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_BlueArcSheetB
BattleFx_BlueArcSheetB:
	.incbin "GRAPHICS/BATTLE_FX/BLUE_ARC_B.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BLUE_ARC_B.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0xaa, 0x30	@ PACKER: left in the buffer by BattleFx_EmberStreakSheet+0xace
	.endif
