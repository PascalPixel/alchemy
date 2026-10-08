	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_JupiterDjinnSheet
BattleFx_JupiterDjinnSheet:
	.incbin "GRAPHICS/BATTLE_FX/JUPITER_DJINN.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/JUPITER_DJINN.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x13	@ PACKER: left in the buffer by BattleFx_MarsDjinnSheet+0x3a3
	.endif
