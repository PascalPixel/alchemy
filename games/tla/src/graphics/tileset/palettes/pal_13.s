	.section .rodata
	.global Tileset_Palette13
Tileset_Palette13:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_13.gbapal.lz"
	.byte 0xa0, 0x05	@ PACKER: left in the buffer by Field_Map106+0x1e2
