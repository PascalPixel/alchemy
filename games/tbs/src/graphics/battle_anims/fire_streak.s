	.section .rodata
	.global BattleAnim_FireStreakSheet
BattleAnim_FireStreakSheet:
	.incbin "games/tbs/src/graphics/battle_anims/fire_streak.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/fire_streak.bitmap.lz"
	.byte 0x00, 0x48	@ PACKER: left in the buffer by BattleAnim_WhirlwindSheet+0x15c2
