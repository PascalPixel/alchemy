	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_DustPuffSheet
BattleFx_DustPuffSheet:
	.incbin "GRAPHICS/BATTLE_FX/DUST_PUFF.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/DUST_PUFF.bitmap.lz"
	.ifeq GAME_TLA
	.balign 4, 0
	.endif
