	.section .rodata
	.global Tileset_Set05Palette
Tileset_Set05Palette:
	.incbin "graphics/tileset/set_05/palette.gbapal.lz"
	.byte 0x64, 0x36, 0x80	@ PACKER: left in the buffer by Field_Map006+0x18d
