	.section .rodata
	.global Tileset_Set70Palette
Tileset_Set70Palette:
	.incbin "games/tla/src/graphics/tileset/set_70/palette.gbapal.lz"
	.byte 0x80, 0xa0	@ PACKER: left in the buffer by Field_Map147+0xe2
