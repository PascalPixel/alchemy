@ BattleFx_RainSheet: a 64x340 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_RainSheet
BattleFx_RainSheet:
	.incbin "GRAPHICS/BATTLE_FX/RAIN.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/RAIN.bitmap.lz"
