	.section .rodata
	.global BattleAnim_VioletStreakSheet
BattleAnim_VioletStreakSheet:
	.incbin "games/tbs/src/graphics/battle_fx/violet_streak.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/violet_streak.bitmap.lz"
	.byte 0x5b	@ PACKER: left in the buffer by BattleAnim_SpiralSheet+0xc8b
