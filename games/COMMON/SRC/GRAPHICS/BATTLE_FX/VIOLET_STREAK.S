@ BattleFx_VioletStreakSheet: a 32x384 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_VioletStreakSheet
BattleFx_VioletStreakSheet:
	.incbin "GRAPHICS/BATTLE_FX/VIOLET_STREAK.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/VIOLET_STREAK.bitmap.lz"
