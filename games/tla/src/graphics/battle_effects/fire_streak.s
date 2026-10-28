	.section .rodata
	.global BattleEffect_FireStreakSheet
BattleEffect_FireStreakSheet:
	.incbin "games/tla/src/graphics/battle_effects/fire_streak.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/fire_streak.bitmap.lz"
	.byte 0x00, 0x48	@ PACKER: left in the buffer by BattleEffect_WhirlwindSheet+0x15c2
