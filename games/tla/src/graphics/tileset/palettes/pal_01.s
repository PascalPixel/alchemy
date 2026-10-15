	.section .rodata
	.global Tileset_Palette01
Tileset_Palette01:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_01.gbapal.lz"
	.byte 0xf0, 0x21, 0x38	@ PACKER: left in the buffer by Field_Map046+0x165
