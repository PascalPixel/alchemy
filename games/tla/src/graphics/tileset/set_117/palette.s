	.section .rodata
	.global Tileset_Set117Palette
Tileset_Set117Palette:
	.incbin "games/tla/src/graphics/tileset/set_117/palette.gbapal.lz"
	.byte 0x74, 0x6c, 0x5a	@ PACKER: left in the buffer by Field_Map287+0x121
