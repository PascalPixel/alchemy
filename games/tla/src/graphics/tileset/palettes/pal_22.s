	.section .rodata
	.global Tileset_Palette22
Tileset_Palette22:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_22.gbapal.lz"
	.byte 0xac, 0x2f	@ PACKER: left in the buffer by Field_Map127+0x116
