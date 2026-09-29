@ BattleFx_MarsDjinnSmallSheet: a 40x20 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_MarsDjinnSmallSheet
BattleFx_MarsDjinnSmallSheet:
	.incbin "GRAPHICS/BATTLE_FX/MARS_DJINN_SMALL.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/MARS_DJINN_SMALL.bitmap.lz"
