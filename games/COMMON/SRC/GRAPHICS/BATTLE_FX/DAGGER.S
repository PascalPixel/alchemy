@ BattleFx_DaggerSheet: a 24x280 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_DaggerSheet
BattleFx_DaggerSheet:
	.incbin "GRAPHICS/BATTLE_FX/DAGGER.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/DAGGER.bitmap.lz"
