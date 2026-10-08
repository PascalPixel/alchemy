	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_JupiterDjinnSmallSheet
BattleFx_JupiterDjinnSmallSheet:
	.incbin "GRAPHICS/BATTLE_FX/JUPITER_DJINN_SMALL.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/JUPITER_DJINN_SMALL.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x0b, 0x1f	@ PACKER: left in the buffer by BattleFx_JupiterDjinnSheet+0x25a
	.endif
