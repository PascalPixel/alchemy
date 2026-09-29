@ BattleFx_VioletPaletteD: a 1x1 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_VioletPaletteD
BattleFx_VioletPaletteD:
	.incbin "GRAPHICS/BATTLE_FX/VIOLET_PALETTE_D.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/VIOLET_PALETTE_D.bitmap.lz"
