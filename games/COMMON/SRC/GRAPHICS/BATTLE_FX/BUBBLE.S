	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_BubbleSheet
BattleFx_BubbleSheet:
	.incbin "GRAPHICS/BATTLE_FX/BUBBLE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BUBBLE.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x14, 0xb1, 0x50	@ PACKER: left in the buffer by BattleFx_YellowOrbSheet+0x381
	.endif
