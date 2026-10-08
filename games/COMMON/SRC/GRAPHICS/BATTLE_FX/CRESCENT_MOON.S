	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_CrescentMoonSheet
BattleFx_CrescentMoonSheet:
	.incbin "GRAPHICS/BATTLE_FX/CRESCENT_MOON.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/CRESCENT_MOON.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x2b	@ PACKER: left in the buffer by BattleFx_VioletStreakSheet+0x4cf
	.endif
