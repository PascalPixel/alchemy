	.section .rodata
	.global BattleEffect_CrescentRowSheet
BattleEffect_CrescentRowSheet:
	.incbin "games/tla/src/graphics/battle_effects/crescent_row.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/crescent_row.bitmap.lz"
	.byte 0x26	@ PACKER: left in the buffer by BattleEffect_PetalSheet+0x9bb
