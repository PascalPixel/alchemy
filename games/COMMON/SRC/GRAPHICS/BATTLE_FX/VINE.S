	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_VineSheet
BattleFx_VineSheet:
	.incbin "GRAPHICS/BATTLE_FX/VINE_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/VINE.parts.lz"
	.ifeq GAME_TLA
	.byte 0xbe	@ PACKER: left in the buffer by BattleFx_WhirlwindSheet+0xdcb
	.endif
