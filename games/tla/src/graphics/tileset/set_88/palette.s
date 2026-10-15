	.section .rodata
	.global Tileset_Set88Palette
Tileset_Set88Palette:
	.incbin "games/tla/src/graphics/tileset/set_88/palette.gbapal.lz"
	.byte 0xfe, 0x07	@ PACKER: left in the buffer by Field_Map202+0x17e
