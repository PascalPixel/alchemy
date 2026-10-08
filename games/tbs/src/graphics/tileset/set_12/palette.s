	.section .rodata
	.global Tileset_Set12Palette
Tileset_Set12Palette:
	.incbin "graphics/tileset/set_12/palette.gbapal.lz"
	.byte 0xb1	@ PACKER: left in the buffer by Field_Map014+0x127
