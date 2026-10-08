	.section .rodata
	.global Tileset_Set66Palette
Tileset_Set66Palette:
	.incbin "graphics/tileset/set_66/palette.gbapal.lz"
	.byte 0x43, 0x11, 0xf8	@ PACKER: left in the buffer by Field_Map169+0xe5
