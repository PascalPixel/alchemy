	.section .rodata
	.global Tileset_Palette21
Tileset_Palette21:
	.incbin "graphics/tileset/palettes/pal_21.gbapal.lz"
	.byte 0xfe, 0xa6	@ PACKER: left in the buffer by Field_Map115+0x1ba
