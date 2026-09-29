@ BattleFx_SmokeBandSheet: a 64x32 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_SmokeBandSheet
BattleFx_SmokeBandSheet:
	.incbin "GRAPHICS/BATTLE_FX/SMOKE_BAND.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SMOKE_BAND.bitmap.lz"
