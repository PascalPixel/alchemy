@ BattleFx_CyanPalette: a 1x1 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_CyanPalette
BattleFx_CyanPalette:
	.incbin "GRAPHICS/BATTLE_FX/CYAN_PALETTE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/CYAN_PALETTE.bitmap.lz"
