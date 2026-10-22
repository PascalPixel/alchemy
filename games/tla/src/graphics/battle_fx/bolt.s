	.section .rodata
	.global BattleEffect_BoltSheet
BattleEffect_BoltSheet:
	.incbin "games/tla/src/graphics/battle_fx/bolt.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/bolt.bitmap.lz"
	.byte 0xcb, 0x31, 0x16	@ PACKER: left in the buffer by BattleEffect_WhirlBeamSheet+0x1be1
