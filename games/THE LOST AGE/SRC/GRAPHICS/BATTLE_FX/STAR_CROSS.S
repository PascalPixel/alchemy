@ BattleFx_StarCrossSheet: a 64x64 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_StarCrossSheet
BattleFx_StarCrossSheet:
	.incbin "GRAPHICS/BATTLE_FX/STAR_CROSS.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/STAR_CROSS.bitmap.lz"
