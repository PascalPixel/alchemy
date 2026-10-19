	.section .rodata
	.global BattleFx_JupiterDjinnSmallSheet
BattleFx_JupiterDjinnSmallSheet:
	.incbin "games/tla/src/graphics/battle_fx/jupiter_djinn_small.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/jupiter_djinn_small.bitmap.lz"
	.byte 0x0b, 0x1f	@ PACKER: left in the buffer by BattleFx_JupiterDjinnSheet+0x25a
