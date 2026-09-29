@ BattleFx_YellowPaletteB: a 1x1 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_YellowPaletteB
BattleFx_YellowPaletteB:
	.incbin "GRAPHICS/BATTLE_FX/YELLOW_PALETTE_B.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/YELLOW_PALETTE_B.bitmap.lz"
