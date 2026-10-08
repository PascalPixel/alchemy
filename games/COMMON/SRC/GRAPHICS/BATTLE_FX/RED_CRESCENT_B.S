	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_RedCrescentSheetB
BattleFx_RedCrescentSheetB:
	.incbin "GRAPHICS/BATTLE_FX/RED_CRESCENT_B.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/RED_CRESCENT_B.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0xf0, 0x79	@ PACKER: left in the buffer by BattleFx_BlueBeastSheet+0x3ea
	.endif
