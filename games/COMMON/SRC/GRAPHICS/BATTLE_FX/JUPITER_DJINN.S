@ BattleFx_JupiterDjinnSheet: a 40x40 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_JupiterDjinnSheet
BattleFx_JupiterDjinnSheet:
	.incbin "GRAPHICS/BATTLE_FX/JUPITER_DJINN.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/JUPITER_DJINN.bitmap.lz"
