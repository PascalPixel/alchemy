	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_MarsDjinnSmallSheet
BattleFx_MarsDjinnSmallSheet:
	.incbin "GRAPHICS/BATTLE_FX/MARS_DJINN_SMALL.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/MARS_DJINN_SMALL.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_MarsDjinnSheet at +0x289 */
	.byte 0x8a, 0x14, 0x39
	.endif
