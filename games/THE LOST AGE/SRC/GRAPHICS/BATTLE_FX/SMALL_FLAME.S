@ BattleFx_SmallFlameSheet: a 16x256 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_SmallFlameSheet
BattleFx_SmallFlameSheet:
	.incbin "GRAPHICS/BATTLE_FX/SMALL_FLAME.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SMALL_FLAME.bitmap.lz"
