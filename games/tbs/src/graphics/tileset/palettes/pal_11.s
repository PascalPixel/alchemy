	.section .rodata
	.global Tileset_Palette11
Tileset_Palette11:
	.incbin "graphics/tileset/palettes/pal_11.gbapal.lz"
	.byte 0xe7, 0x99	@ PACKER: left in the buffer by Field_Map104+0x1d2
