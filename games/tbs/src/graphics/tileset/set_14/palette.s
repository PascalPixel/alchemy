	.section .rodata
	.global Tileset_Set14Palette
Tileset_Set14Palette:
	.incbin "graphics/tileset/set_14/palette.gbapal.lz"
	.byte 0xc8	@ PACKER: left in the buffer by Field_Map017+0x183
