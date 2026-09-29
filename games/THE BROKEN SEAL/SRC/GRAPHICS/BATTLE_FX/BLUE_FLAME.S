@ BattleFx_BlueFlameSheet: a 34x117 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_BlueFlameSheet
BattleFx_BlueFlameSheet:
	.incbin "GRAPHICS/BATTLE_FX/BLUE_FLAME.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BLUE_FLAME.bitmap.lz"
