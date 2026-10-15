	.section .rodata
	.global BattleFx_BoltSheet
BattleFx_BoltSheet:
	.incbin "games/tla/src/graphics/battle_fx/bolt.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/bolt.bitmap.lz"
	.byte 0xcb, 0x31, 0x16	@ PACKER: left in the buffer by BattleFx_WhirlBeamSheet+0x1be1
