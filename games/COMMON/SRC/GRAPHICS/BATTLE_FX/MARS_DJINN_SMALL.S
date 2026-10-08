	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_MarsDjinnSmallSheet
BattleFx_MarsDjinnSmallSheet:
	.incbin "GRAPHICS/BATTLE_FX/MARS_DJINN_SMALL.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/MARS_DJINN_SMALL.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x8a, 0x14, 0x39	@ PACKER: left in the buffer by BattleFx_MarsDjinnSheet+0x289
	.endif
