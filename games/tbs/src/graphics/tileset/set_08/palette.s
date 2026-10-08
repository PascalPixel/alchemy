	.section .rodata
	.global Tileset_Set08Palette
Tileset_Set08Palette:
	.incbin "graphics/tileset/set_08/palette.gbapal.lz"
	.byte 0xcf, 0x83, 0x07	@ PACKER: left in the buffer by Field_Map009+0x159
