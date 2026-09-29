@ BattleFx_SunCardSheet: a 32x64 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_SunCardSheet
BattleFx_SunCardSheet:
	.incbin "GRAPHICS/BATTLE_FX/SUN_CARD.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SUN_CARD.bitmap.lz"
