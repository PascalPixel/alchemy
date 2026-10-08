	.section .rodata
	.global Tileset_Set07Palette
Tileset_Set07Palette:
	.incbin "graphics/tileset/set_07/palette.gbapal.lz"
	.byte 0x13, 0x3e	@ PACKER: left in the buffer by Field_Map008+0xe2
