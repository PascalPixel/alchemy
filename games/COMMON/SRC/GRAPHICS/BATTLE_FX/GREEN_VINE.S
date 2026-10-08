	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_GreenVineSheet
BattleFx_GreenVineSheet:
	.incbin "GRAPHICS/BATTLE_FX/GREEN_VINE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/GREEN_VINE.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_WispSheet at +0x516 */
	.byte 0x1c, 0xaf
	.endif
