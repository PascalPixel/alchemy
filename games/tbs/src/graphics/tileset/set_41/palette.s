	.section .rodata
	.global Tileset_Set41Palette
Tileset_Set41Palette:
	.incbin "graphics/tileset/set_41/palette.gbapal.lz"
	.byte 0xc2, 0x82, 0x04	@ PACKER: left in the buffer by Field_Map051+0x145
