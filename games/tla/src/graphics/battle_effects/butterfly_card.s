	.section .rodata
	.global BattleEffect_ButterflyCardSheet
BattleEffect_ButterflyCardSheet:
	.incbin "games/tla/src/graphics/battle_effects/butterfly_card.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/butterfly_card.bitmap.lz"
	.byte 0xcc, 0x17, 0x03	@ PACKER: left in the buffer by BattleEffect_BoltCardSheet+0x61d
