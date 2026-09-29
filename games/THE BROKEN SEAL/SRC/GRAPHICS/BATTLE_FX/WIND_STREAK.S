@ BattleFx_WindStreakSheet: the 64-colour palette of its first picture, then its 14 pictures
@ (listed in WIND_STREAK.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_WindStreakSheet
BattleFx_WindStreakSheet:
	.incbin "GRAPHICS/BATTLE_FX/WIND_STREAK_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/WIND_STREAK.parts.lz"
