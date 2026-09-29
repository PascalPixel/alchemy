@ BattleFx_FireStreakSheet: a 48x432 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_FireStreakSheet
BattleFx_FireStreakSheet:
	.incbin "GRAPHICS/BATTLE_FX/FIRE_STREAK.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FIRE_STREAK.bitmap.lz"
