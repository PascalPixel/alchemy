	.section .rodata
	.global BattleAnim_ShieldSheet
BattleAnim_ShieldSheet:
	.incbin "games/tbs/src/graphics/battle_fx/shield.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/shield.bitmap.lz"
	.byte 0x45, 0xb1, 0x12	@ PACKER: left in the buffer by BattleAnim_FireStreakSheet+0xcad
