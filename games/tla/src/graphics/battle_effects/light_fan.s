	.section .rodata
	.global BattleEffect_LightFanSheet
BattleEffect_LightFanSheet:
	.incbin "games/tla/src/graphics/battle_effects/light_fan.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/light_fan.bitmap.lz"
	.byte 0x09, 0x82, 0x76	@ PACKER: left in the buffer by BattleEffect_IceBlockSheet+0x59d
