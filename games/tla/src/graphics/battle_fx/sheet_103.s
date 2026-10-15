	.section .rodata
	.global Resource_Data103
Resource_Data103:
	.incbin "games/tla/src/graphics/battle_fx/sheet_103.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/sheet_103.8bpp.lz"
	.byte 0x31, 0x32, 0x8d	@ PACKER: left in the buffer by Resource_Data0F8+0xb2d
