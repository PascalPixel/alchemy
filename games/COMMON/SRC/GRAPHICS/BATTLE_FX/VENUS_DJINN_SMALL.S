@ BattleFx_VenusDjinnSmallSheet: a 40x20 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_VenusDjinnSmallSheet
BattleFx_VenusDjinnSmallSheet:
	.incbin "GRAPHICS/BATTLE_FX/VENUS_DJINN_SMALL.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/VENUS_DJINN_SMALL.bitmap.lz"
