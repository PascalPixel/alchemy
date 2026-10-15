	.section .rodata
	.global BattleFx_MarsDjinnSmallSheet
BattleFx_MarsDjinnSmallSheet:
	.incbin "games/common/src/graphics/battle_fx/mars_djinn_small.gbapal"
	.incbin "games/common/src/graphics/battle_fx/mars_djinn_small.bitmap.lz"
	.byte 0x8a, 0x14, 0x39	@ PACKER: left in the buffer by BattleFx_MarsDjinnSheet+0x289
