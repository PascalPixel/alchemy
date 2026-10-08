	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_RockWallSheet
BattleFx_RockWallSheet:
	.incbin "GRAPHICS/BATTLE_FX/ROCK_WALL.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/ROCK_WALL.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x07, 0x43, 0x0b	@ PACKER: left in the buffer by BattleFx_VineSheet+0xcd1
	.endif
