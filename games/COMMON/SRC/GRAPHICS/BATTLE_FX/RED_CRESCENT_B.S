	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_RedCrescentSheetB
BattleFx_RedCrescentSheetB:
	.incbin "GRAPHICS/BATTLE_FX/RED_CRESCENT_B.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/RED_CRESCENT_B.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_BlueBeastSheet at +0x3ea */
	.byte 0xf0, 0x79
	.endif
