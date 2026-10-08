	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_MarsDjinnSheet
BattleFx_MarsDjinnSheet:
	.incbin "GRAPHICS/BATTLE_FX/MARS_DJINN.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/MARS_DJINN.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_StarDotSheet at +0x3db */
	.byte 0x04
	.endif
