	.section .rodata
	.global Tileset_Palette69
Tileset_Palette69:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_69.gbapal.lz"
	.byte 0xbc, 0x70	@ PACKER: left in the buffer by Field_Map285+0x172
