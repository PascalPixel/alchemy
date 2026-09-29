@ BattleFx_CrescentMoonSheet: a 24x120 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_CrescentMoonSheet
BattleFx_CrescentMoonSheet:
	.incbin "GRAPHICS/BATTLE_FX/CRESCENT_MOON.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/CRESCENT_MOON.bitmap.lz"
