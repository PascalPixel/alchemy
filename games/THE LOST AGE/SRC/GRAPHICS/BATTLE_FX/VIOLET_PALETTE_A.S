@ BattleFx_VioletPaletteA: a 1x1 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_VioletPaletteA
BattleFx_VioletPaletteA:
	.incbin "GRAPHICS/BATTLE_FX/VIOLET_PALETTE_A.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/VIOLET_PALETTE_A.bitmap.lz"
