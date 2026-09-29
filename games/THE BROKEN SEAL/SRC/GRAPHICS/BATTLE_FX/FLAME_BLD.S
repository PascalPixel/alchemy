@ BattleFx_FlameBladeSheet: a 64x128 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_FlameBladeSheet
BattleFx_FlameBladeSheet:
	.incbin "GRAPHICS/BATTLE_FX/FLAME_BLD.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FLAME_BLD.bitmap.lz"
