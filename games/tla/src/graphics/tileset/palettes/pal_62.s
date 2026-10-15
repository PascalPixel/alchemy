	.section .rodata
	.global Tileset_Palette62
Tileset_Palette62:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_62.gbapal.lz"
	.byte 0x95, 0xe2, 0xb5	@ PACKER: left in the buffer by Field_Map276+0x129
