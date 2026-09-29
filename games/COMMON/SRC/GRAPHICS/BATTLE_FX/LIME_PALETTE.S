@ BattleFx_LimePalette: a 1x1 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_LimePalette
BattleFx_LimePalette:
	.incbin "GRAPHICS/BATTLE_FX/LIME_PALETTE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/LIME_PALETTE.bitmap.lz"
