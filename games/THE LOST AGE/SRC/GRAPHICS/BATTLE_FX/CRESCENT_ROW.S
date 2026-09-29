@ BattleFx_CrescentRowSheet: a 232x30 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_CrescentRowSheet
BattleFx_CrescentRowSheet:
	.incbin "GRAPHICS/BATTLE_FX/CRESCENT_ROW.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/CRESCENT_ROW.bitmap.lz"
