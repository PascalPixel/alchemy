@ BattleFx_BluePalette: a 1x1 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_BluePalette
BattleFx_BluePalette:
	.incbin "GRAPHICS/BATTLE_FX/BLUE_PALETTE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BLUE_PALETTE.bitmap.lz"
