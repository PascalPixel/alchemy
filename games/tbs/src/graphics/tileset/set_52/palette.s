	.section .rodata
	.global Tileset_Set52Palette
Tileset_Set52Palette:
	.incbin "graphics/tileset/set_52/palette.gbapal.lz"
	.byte 0xc0	@ PACKER: left in the buffer by Field_Map094+0x1c7
