	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_VioletCometSheet
BattleFx_VioletCometSheet:
	.incbin "GRAPHICS/BATTLE_FX/VIOLET_COMET_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/VIOLET_COMET.parts.lz"
	.ifeq GAME_TLA
	.byte 0x5e, 0xda, 0x33	@ PACKER: left in the buffer by BattleFx_FirePillarSheetA+0x3e5
	.endif
