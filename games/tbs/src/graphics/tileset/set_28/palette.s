	.section .rodata
	.global Tileset_Set28Palette
Tileset_Set28Palette:
	.incbin "graphics/tileset/set_28/palette.gbapal.lz"
	.byte 0x40, 0x15, 0x08	@ PACKER: left in the buffer by Field_Map036+0x1a1
