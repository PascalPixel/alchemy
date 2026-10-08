	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_CometSheetB
BattleFx_CometSheetB:
	.incbin "GRAPHICS/BATTLE_FX/COMET_B.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/COMET_B.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0xaf	@ PACKER: left in the buffer by BattleFx_ThornSheet+0x2fb
	.endif
