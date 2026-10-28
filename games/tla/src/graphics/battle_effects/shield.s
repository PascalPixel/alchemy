	.section .rodata
	.global BattleEffect_ShieldSheet
BattleEffect_ShieldSheet:
	.incbin "games/tla/src/graphics/battle_effects/shield.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/shield.bitmap.lz"
	.byte 0x45, 0xb1, 0x12	@ PACKER: left in the buffer by BattleEffect_FireStreakSheet+0xcad
