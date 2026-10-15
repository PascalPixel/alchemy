	.section .rodata
	.global Tileset_Palette26
Tileset_Palette26:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_26.gbapal.lz"
	.byte 0x44	@ PACKER: left in the buffer by Tileset_Palette25+0x15b
