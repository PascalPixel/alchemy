	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_RockWallSheet
BattleFx_RockWallSheet:
	.incbin "GRAPHICS/BATTLE_FX/ROCK_WALL.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/ROCK_WALL.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_VineSheet at +0xcd1 */
	.byte 0x07, 0x43, 0x0b
	.endif
