	.section .rodata
	.global Tileset_Palette07
Tileset_Palette07:
	.incbin "graphics/tileset/palettes/pal_07.gbapal.lz"
	.byte 0x14, 0x26	@ PACKER: left in the buffer by Field_Map088+0x162
