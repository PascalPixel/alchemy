	.section .rodata
	.global BattleAnim_CrescentMoonSheet
BattleAnim_CrescentMoonSheet:
	.incbin "games/tbs/src/graphics/battle_fx/crescent_moon.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/crescent_moon.bitmap.lz"
	.byte 0x2b	@ PACKER: left in the buffer by BattleAnim_VioletStreakSheet+0x4cf
