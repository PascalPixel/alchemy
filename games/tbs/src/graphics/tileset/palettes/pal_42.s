	.section .rodata
	.global Tileset_Palette42
Tileset_Palette42:
	.incbin "common/graphics/tileset/palettes/pal_42.gbapal.lz"
	.byte 0x94, 0x1c	@ PACKER: left in the buffer by Field_Map154+0x1b6
