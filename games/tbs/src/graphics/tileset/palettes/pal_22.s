	.section .rodata
	.global Tileset_Palette22
Tileset_Palette22:
	.incbin "graphics/tileset/palettes/pal_22.gbapal.lz"
	.byte 0x50, 0x42, 0x7d	@ PACKER: left in the buffer by Field_Map116+0x171
