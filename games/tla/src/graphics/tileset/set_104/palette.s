	.section .rodata
	.global Tileset_Set104Palette
Tileset_Set104Palette:
	.incbin "games/tla/src/graphics/tileset/set_104/palette.gbapal.lz"
	.byte 0x00, 0x1d, 0x0f	@ PACKER: left in the buffer by Field_Map240+0x179
