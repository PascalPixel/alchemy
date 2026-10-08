	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_RockSpireSheet
BattleFx_RockSpireSheet:
	.incbin "GRAPHICS/BATTLE_FX/ROCK_SPIRE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/ROCK_SPIRE.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_FirePillarSheetB at +0x7a1 */
	.byte 0xa0, 0x20, 0x72
	.endif
