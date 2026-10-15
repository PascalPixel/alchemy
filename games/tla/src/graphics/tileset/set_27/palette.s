	.section .rodata
	.global Tileset_Set27Palette
Tileset_Set27Palette:
	.incbin "games/tla/src/graphics/tileset/set_27/palette.gbapal.lz"
	.byte 0x60, 0x3f, 0xbe	@ PACKER: left in the buffer by Field_Map044+0x115
