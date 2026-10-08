	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_VioletCometSheet
BattleFx_VioletCometSheet:
	.incbin "GRAPHICS/BATTLE_FX/VIOLET_COMET_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/VIOLET_COMET.parts.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_FirePillarSheetA at +0x3e5 */
	.byte 0x5e, 0xda, 0x33
	.endif
