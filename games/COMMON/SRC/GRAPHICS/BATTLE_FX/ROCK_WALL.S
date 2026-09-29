@ BattleFx_RockWallSheet: a 40x96 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_RockWallSheet
BattleFx_RockWallSheet:
	.incbin "GRAPHICS/BATTLE_FX/ROCK_WALL.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/ROCK_WALL.bitmap.lz"
