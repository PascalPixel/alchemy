	.section .rodata
	.global BattleFx_ShieldSheet
BattleFx_ShieldSheet:
	.incbin "games/tbs/src/graphics/battle_fx/shield.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/shield.bitmap.lz"
	.byte 0x45, 0xb1, 0x12	@ PACKER: left in the buffer by BattleFx_FireStreakSheet+0xcad
