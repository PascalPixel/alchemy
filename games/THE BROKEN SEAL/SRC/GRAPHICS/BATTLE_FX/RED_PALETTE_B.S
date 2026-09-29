@ BattleFx_RedPaletteB: a 1x1 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_RedPaletteB
BattleFx_RedPaletteB:
	.incbin "GRAPHICS/BATTLE_FX/RED_PALETTE_B.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/RED_PALETTE_B.bitmap.lz"
