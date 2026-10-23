	.section .rodata
	.global BattleAnim_LightFanSheet
BattleAnim_LightFanSheet:
	.incbin "games/tbs/src/graphics/battle_fx/light_fan.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/light_fan.bitmap.lz"
	.byte 0x09, 0x82, 0x76	@ PACKER: left in the buffer by BattleAnim_IceBlockSheet+0x59d
