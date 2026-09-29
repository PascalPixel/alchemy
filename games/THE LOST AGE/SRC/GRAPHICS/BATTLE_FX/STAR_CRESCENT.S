@ BattleFx_StarCrescentSheet: a 120x240 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_StarCrescentSheet
BattleFx_StarCrescentSheet:
	.incbin "GRAPHICS/BATTLE_FX/STAR_CRESCENT.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/STAR_CRESCENT.bitmap.lz"
