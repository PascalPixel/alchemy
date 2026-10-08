	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_GlowOrbSheet
BattleFx_GlowOrbSheet:
	.incbin "GRAPHICS/BATTLE_FX/GLOW_ORB.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/GLOW_ORB.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0xe0	@ PACKER: left in the buffer by BattleFx_EmberStreakSheet+0xb3b
	.endif
