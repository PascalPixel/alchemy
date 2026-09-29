@ BattleFx_FeatherSheet: a 64x64 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_FeatherSheet
BattleFx_FeatherSheet:
	.incbin "GRAPHICS/BATTLE_FX/FEATHER.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FEATHER.bitmap.lz"
