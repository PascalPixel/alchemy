	.section .rodata
	.global Tileset_Set52Palette
Tileset_Set52Palette:
	.incbin "games/tla/src/graphics/tileset/set_52/palette.gbapal.lz"
	.byte 0x3c, 0x88, 0x03	@ PACKER: left in the buffer by Field_Map103+0x11d
