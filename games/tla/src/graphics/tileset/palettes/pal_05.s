	.section .rodata
	.global Tileset_Palette05
Tileset_Palette05:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_05.gbapal.lz"
	.byte 0xf0	@ PACKER: left in the buffer by Field_Map057+0xf3
