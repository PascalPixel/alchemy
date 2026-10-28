	.section .rodata
	.global BattleEffect_StarCrescentSheet
BattleEffect_StarCrescentSheet:
	.incbin "games/tla/src/graphics/battle_effects/star_crescent.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/star_crescent.bitmap.lz"
	.byte 0x03	@ PACKER: left in the buffer by BattleEffect_GreyCrescentSheet+0xadb
