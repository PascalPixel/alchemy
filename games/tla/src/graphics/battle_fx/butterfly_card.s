	.section .rodata
	.global BattleFx_ButterflyCardSheet
BattleFx_ButterflyCardSheet:
	.incbin "games/tla/src/graphics/battle_fx/butterfly_card.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/butterfly_card.bitmap.lz"
	.byte 0xcc, 0x17, 0x03	@ PACKER: left in the buffer by BattleFx_BoltCardSheet+0x61d
