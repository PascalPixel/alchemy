	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_CometSheetA
BattleFx_CometSheetA:
	.incbin "GRAPHICS/BATTLE_FX/COMET_A.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/COMET_A.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_ThornSheet at +0x21b */
	.byte 0x2a
	.endif
