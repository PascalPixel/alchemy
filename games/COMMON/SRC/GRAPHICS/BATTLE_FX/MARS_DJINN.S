	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_MarsDjinnSheet
BattleFx_MarsDjinnSheet:
	.incbin "GRAPHICS/BATTLE_FX/MARS_DJINN.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/MARS_DJINN.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x04	@ PACKER: left in the buffer by BattleFx_StarDotSheet+0x3db
	.endif
