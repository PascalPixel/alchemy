	.section .rodata
	.global Tileset_Palette60
Tileset_Palette60:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_60.gbapal.lz"
	.byte 0x0f	@ PACKER: left in the buffer by Field_Map257+0x1b7
