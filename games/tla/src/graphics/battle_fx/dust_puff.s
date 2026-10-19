	.section .rodata
	.global BattleFx_DustPuffSheet
BattleFx_DustPuffSheet:
	.incbin "games/tla/src/graphics/battle_fx/dust_puff.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/dust_puff.bitmap.lz"
	.byte 0x7e, 0xe0, 0x98	@ PACKER: left in the buffer by BattleFx_LightFanSheet+0x339
