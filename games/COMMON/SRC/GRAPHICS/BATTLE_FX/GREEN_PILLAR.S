	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_GreenPillarSheet
BattleFx_GreenPillarSheet:
	.incbin "GRAPHICS/BATTLE_FX/GREEN_PILLAR_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/GREEN_PILLAR.parts.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_WhirlwindSheet at +0x75d */
	.byte 0x21, 0xf0, 0x28
	.endif
