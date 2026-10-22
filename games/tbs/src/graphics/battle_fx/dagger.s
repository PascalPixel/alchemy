	.section .rodata
	.global BattleEffect_DaggerSheet
BattleEffect_DaggerSheet:
	.incbin "games/tbs/src/graphics/battle_fx/dagger.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/dagger.bitmap.lz"
	.byte 0x0a, 0x77, 0x19	@ PACKER: left in the buffer by BattleEffect_ShieldSheet+0xb45
