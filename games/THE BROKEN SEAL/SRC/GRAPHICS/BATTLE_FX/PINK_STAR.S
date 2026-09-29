@ BattleFx_PinkStarSheet: a 16x96 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_PinkStarSheet
BattleFx_PinkStarSheet:
	.incbin "GRAPHICS/BATTLE_FX/PINK_STAR.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/PINK_STAR.bitmap.lz"
