	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_WispSheet
BattleFx_WispSheet:
	.incbin "GRAPHICS/BATTLE_FX/WISP.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/WISP.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_SmokeSheet at +0x7ae */
	.byte 0x2e, 0x06
	.endif
