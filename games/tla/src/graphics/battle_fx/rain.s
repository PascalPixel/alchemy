	.section .rodata
	.global BattleFx_RainSheet
BattleFx_RainSheet:
	.incbin "games/tla/src/graphics/battle_fx/rain.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/rain.bitmap.lz"
	.byte 0x27	@ PACKER: left in the buffer by BattleFx_StarCrescentSheet+0x89f
