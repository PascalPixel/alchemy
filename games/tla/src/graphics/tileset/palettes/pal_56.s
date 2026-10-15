	.section .rodata
	.global Tileset_Palette56
Tileset_Palette56:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_56.gbapal.lz"
	.byte 0x4c	@ PACKER: left in the buffer by Field_Map250+0x127
