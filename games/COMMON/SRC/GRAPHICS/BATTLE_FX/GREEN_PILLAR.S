	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_GreenPillarSheet
BattleFx_GreenPillarSheet:
	.incbin "GRAPHICS/BATTLE_FX/GREEN_PILLAR_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/GREEN_PILLAR.parts.lz"
	.ifeq GAME_TLA
	.byte 0x21, 0xf0, 0x28	@ PACKER: left in the buffer by BattleFx_WhirlwindSheet+0x75d
	.endif
