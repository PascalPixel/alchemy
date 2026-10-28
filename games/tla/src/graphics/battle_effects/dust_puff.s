	.section .rodata
	.global BattleEffect_DustPuffSheet
BattleEffect_DustPuffSheet:
	.incbin "games/tla/src/graphics/battle_effects/dust_puff.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/dust_puff.bitmap.lz"
	.byte 0x7e, 0xe0, 0x98	@ PACKER: left in the buffer by BattleEffect_LightFanSheet+0x339
