	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_JupiterDjinnSheet
BattleFx_JupiterDjinnSheet:
	.incbin "GRAPHICS/BATTLE_FX/JUPITER_DJINN.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/JUPITER_DJINN.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_MarsDjinnSheet at +0x3a3 */
	.byte 0x13
	.endif
