@ BattleFx_GreenVineSheet: a 32x160 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_GreenVineSheet
BattleFx_GreenVineSheet:
	.incbin "GRAPHICS/BATTLE_FX/GREEN_VINE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/GREEN_VINE.bitmap.lz"
