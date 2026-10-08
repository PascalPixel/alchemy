	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_BubbleSheet
BattleFx_BubbleSheet:
	.incbin "GRAPHICS/BATTLE_FX/BUBBLE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BUBBLE.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_YellowOrbSheet at +0x381 */
	.byte 0x14, 0xb1, 0x50
	.endif
