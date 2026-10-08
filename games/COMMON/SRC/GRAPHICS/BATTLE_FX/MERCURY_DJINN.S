	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_MercuryDjinnSheet
BattleFx_MercuryDjinnSheet:
	.incbin "GRAPHICS/BATTLE_FX/MERCURY_DJINN.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/MERCURY_DJINN.bitmap.lz"
	.ifeq GAME_TLA
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
