	.section .rodata
	.global Tileset_Palette32
Tileset_Palette32:
	.incbin "graphics/tileset/palettes/pal_32.gbapal.lz"
	.byte 0xf9, 0x56	@ PACKER: left in the buffer by Field_Map133+0x16a
