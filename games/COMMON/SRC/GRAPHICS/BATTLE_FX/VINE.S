	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_VineSheet
BattleFx_VineSheet:
	.incbin "GRAPHICS/BATTLE_FX/VINE_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/VINE.parts.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_WhirlwindSheet at +0xdcb */
	.byte 0xbe
	.endif
