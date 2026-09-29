@ BattleFx_RedPaletteA: a 1x1 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_RedPaletteA
BattleFx_RedPaletteA:
	.incbin "GRAPHICS/BATTLE_FX/RED_PALETTE_A.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/RED_PALETTE_A.bitmap.lz"
