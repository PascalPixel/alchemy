	.section .rodata
	.global BattleFx_DaggerSheet
BattleFx_DaggerSheet:
	.incbin "games/tla/src/graphics/battle_fx/dagger.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/dagger.bitmap.lz"
	.byte 0x0a, 0x77, 0x19	@ PACKER: left in the buffer by BattleFx_ShieldSheet+0xb45
