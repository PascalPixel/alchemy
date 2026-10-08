	.section .rodata
	.global Tileset_Set11Palette
Tileset_Set11Palette:
	.incbin "graphics/tileset/set_11/palette.gbapal.lz"
	.byte 0x43	@ PACKER: left in the buffer by Field_Map013+0x113
