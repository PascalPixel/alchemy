	.section .rodata
	.global BattleFx_LightFanSheet
BattleFx_LightFanSheet:
	.incbin "games/tla/src/graphics/battle_fx/light_fan.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/light_fan.bitmap.lz"
	.byte 0x09, 0x82, 0x76	@ PACKER: left in the buffer by BattleFx_IceBlockSheet+0x59d
