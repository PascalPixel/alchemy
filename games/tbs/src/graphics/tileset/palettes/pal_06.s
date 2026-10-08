	.section .rodata
	.global Tileset_Palette06
Tileset_Palette06:
	.incbin "graphics/tileset/palettes/pal_06.gbapal.lz"
	.byte 0x2a, 0x02, 0x98	@ PACKER: left in the buffer by Field_Map086+0x15d
