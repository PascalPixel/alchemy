	.section .rodata
	.global BattleEffect_CrescentMoonSheet
BattleEffect_CrescentMoonSheet:
	.incbin "games/tla/src/graphics/battle_effects/crescent_moon.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/crescent_moon.bitmap.lz"
	.byte 0x2b	@ PACKER: left in the buffer by BattleEffect_VioletStreakSheet+0x4cf
