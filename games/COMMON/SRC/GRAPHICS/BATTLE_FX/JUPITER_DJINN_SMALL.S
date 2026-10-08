	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_JupiterDjinnSmallSheet
BattleFx_JupiterDjinnSmallSheet:
	.incbin "GRAPHICS/BATTLE_FX/JUPITER_DJINN_SMALL.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/JUPITER_DJINN_SMALL.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_JupiterDjinnSheet at +0x25a */
	.byte 0x0b, 0x1f
	.endif
