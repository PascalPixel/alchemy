	.section .rodata
	.global Tileset_Palette04
Tileset_Palette04:
	.incbin "graphics/tileset/palettes/pal_04.gbapal.lz"
	.byte 0xdb, 0x0e, 0xea	@ PACKER: left in the buffer by Field_Map074+0x195
