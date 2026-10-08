	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_FlameballSheet
BattleFx_FlameballSheet:
	.incbin "GRAPHICS/BATTLE_FX/FLAMEBALL.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FLAMEBALL.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_FirePillarSheetA at +0x557 */
	.byte 0x83
	.endif
