@ BattleFx_PinkPalette: a 1x1 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_PinkPalette
BattleFx_PinkPalette:
	.incbin "GRAPHICS/BATTLE_FX/PINK_PALETTE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/PINK_PALETTE.bitmap.lz"
