	.section .rodata
	.global BattleFx_MarsDjinnSheet
BattleFx_MarsDjinnSheet:
	.incbin "games/tbs/src/graphics/battle_fx/mars_djinn.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/mars_djinn.bitmap.lz"
	.byte 0x04	@ PACKER: left in the buffer by BattleFx_StarDotSheet+0x3db
