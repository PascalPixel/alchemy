	.section .rodata
	.global BattleEffect_VioletStreakSheet
BattleEffect_VioletStreakSheet:
	.incbin "games/tla/src/graphics/battle_effects/violet_streak.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/violet_streak.bitmap.lz"
	.byte 0x5b	@ PACKER: left in the buffer by BattleEffect_SpiralSheet+0xc8b
