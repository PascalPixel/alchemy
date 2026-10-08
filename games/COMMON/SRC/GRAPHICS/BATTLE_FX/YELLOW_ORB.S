	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_YellowOrbSheet
BattleFx_YellowOrbSheet:
	.incbin "GRAPHICS/BATTLE_FX/YELLOW_ORB.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/YELLOW_ORB.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x01, 0x9d	@ PACKER: left in the buffer by BattleFx_LavaOrbSheet+0xa12
	.endif
