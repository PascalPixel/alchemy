	.section .rodata
	.global BattleEffect_DaggerSheet
BattleEffect_DaggerSheet:
	.incbin "games/tla/src/graphics/battle_effects/dagger.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/dagger.bitmap.lz"
	.byte 0x0a, 0x77, 0x19	@ PACKER: left in the buffer by BattleEffect_ShieldSheet+0xb45
