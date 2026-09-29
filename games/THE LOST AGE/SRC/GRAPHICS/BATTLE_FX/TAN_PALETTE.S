@ BattleFx_TanPalette: a 1x1 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_TanPalette
BattleFx_TanPalette:
	.incbin "GRAPHICS/BATTLE_FX/TAN_PALETTE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/TAN_PALETTE.bitmap.lz"
