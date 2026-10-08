	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_VioletStreakSheet
BattleFx_VioletStreakSheet:
	.incbin "GRAPHICS/BATTLE_FX/VIOLET_STREAK.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/VIOLET_STREAK.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x5b	@ PACKER: left in the buffer by BattleFx_SpiralSheet+0xc8b
	.endif
