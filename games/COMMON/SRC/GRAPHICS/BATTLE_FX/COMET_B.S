	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_CometSheetB
BattleFx_CometSheetB:
	.incbin "GRAPHICS/BATTLE_FX/COMET_B.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/COMET_B.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_ThornSheet at +0x2fb */
	.byte 0xaf
	.endif
