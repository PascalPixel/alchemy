	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_GreenVineSheet
BattleFx_GreenVineSheet:
	.incbin "GRAPHICS/BATTLE_FX/GREEN_VINE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/GREEN_VINE.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x1c, 0xaf	@ PACKER: left in the buffer by BattleFx_WispSheet+0x516
	.endif
