	.section .rodata
	.global Tileset_Palette16
Tileset_Palette16:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_16.gbapal.lz"
	.byte 0xe6	@ PACKER: left in the buffer by Field_Map110+0xe3
