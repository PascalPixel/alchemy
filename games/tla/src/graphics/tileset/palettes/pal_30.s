	.section .rodata
	.global Tileset_Palette30
Tileset_Palette30:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_30.gbapal.lz"
	.byte 0x22, 0xb6, 0x05	@ PACKER: left in the buffer by Field_Map172+0x175
