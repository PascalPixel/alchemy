@ BattleFx_RedPaletteC: a 1x1 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_RedPaletteC
BattleFx_RedPaletteC:
	.incbin "GRAPHICS/BATTLE_FX/RED_PALETTE_C.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/RED_PALETTE_C.bitmap.lz"
