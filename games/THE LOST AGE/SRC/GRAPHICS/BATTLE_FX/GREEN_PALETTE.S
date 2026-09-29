@ BattleFx_GreenPalette: a 1x1 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_GreenPalette
BattleFx_GreenPalette:
	.incbin "GRAPHICS/BATTLE_FX/GREEN_PALETTE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/GREEN_PALETTE.bitmap.lz"
