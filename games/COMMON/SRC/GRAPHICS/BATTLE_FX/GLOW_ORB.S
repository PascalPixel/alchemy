	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_GlowOrbSheet
BattleFx_GlowOrbSheet:
	.incbin "GRAPHICS/BATTLE_FX/GLOW_ORB.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/GLOW_ORB.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_EmberStreakSheet at +0xb3b */
	.byte 0xe0
	.endif
