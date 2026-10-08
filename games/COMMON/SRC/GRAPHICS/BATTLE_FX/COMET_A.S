	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_CometSheetA
BattleFx_CometSheetA:
	.incbin "GRAPHICS/BATTLE_FX/COMET_A.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/COMET_A.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x2a	@ PACKER: left in the buffer by BattleFx_ThornSheet+0x21b
	.endif
