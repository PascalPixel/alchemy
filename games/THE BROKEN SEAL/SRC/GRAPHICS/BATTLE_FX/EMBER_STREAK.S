@ BattleFx_EmberStreakSheet: the 64-colour palette of its first picture, then its 6 pictures
@ (listed in EMBER_STREAK.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_EmberStreakSheet
BattleFx_EmberStreakSheet:
	.incbin "GRAPHICS/BATTLE_FX/EMBER_STREAK_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/EMBER_STREAK.parts.lz"
