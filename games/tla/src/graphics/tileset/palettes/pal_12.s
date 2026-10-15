	.section .rodata
	.global Tileset_Palette12
Tileset_Palette12:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_12.gbapal.lz"
	.byte 0xe1, 0x37, 0x03	@ PACKER: left in the buffer by Field_Map100+0x131
