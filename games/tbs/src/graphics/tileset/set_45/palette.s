	.section .rodata
	.global Tileset_Set45Palette
Tileset_Set45Palette:
	.incbin "graphics/tileset/set_45/palette.gbapal.lz"
	.byte 0xa8, 0x80, 0x60	@ PACKER: left in the buffer by Field_Map061+0x1d9
