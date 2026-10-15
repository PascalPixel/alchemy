	.section .rodata
	.global BattleFx_LightBurstSheet
BattleFx_LightBurstSheet:
	.incbin "games/tla/src/graphics/battle_fx/light_burst.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/light_burst.bitmap.lz"
	.byte 0x38, 0x01, 0x26	@ PACKER: left in the buffer by BattleFx_FeatherSheet+0x629
