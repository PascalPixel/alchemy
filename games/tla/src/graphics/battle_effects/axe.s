	.section .rodata
	.global BattleEffect_AxeSheet
BattleEffect_AxeSheet:
	.incbin "games/tla/src/graphics/battle_effects/axe.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/axe.bitmap.lz"
	.byte 0x10, 0x00	@ PACKER: left in the buffer by BattleEffect_ArcFragmentsSheet+0x316
