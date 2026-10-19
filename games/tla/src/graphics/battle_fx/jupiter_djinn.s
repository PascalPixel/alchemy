	.section .rodata
	.global BattleFx_JupiterDjinnSheet
BattleFx_JupiterDjinnSheet:
	.incbin "games/tla/src/graphics/battle_fx/jupiter_djinn.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/jupiter_djinn.bitmap.lz"
	.byte 0x13	@ PACKER: left in the buffer by BattleFx_MarsDjinnSheet+0x3a3
