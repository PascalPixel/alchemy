	.section .rodata
	.global BattleEffect_LightBurstSheet
BattleEffect_LightBurstSheet:
	.incbin "games/tla/src/graphics/battle_effects/light_burst.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/light_burst.bitmap.lz"
	.byte 0x38, 0x01, 0x26	@ PACKER: left in the buffer by BattleEffect_FeatherSheet+0x629
