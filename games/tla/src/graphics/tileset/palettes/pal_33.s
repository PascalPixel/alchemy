	.section .rodata
	.global Tileset_Palette33
Tileset_Palette33:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_33.gbapal.lz"
	.byte 0x85, 0x1e	@ PACKER: left in the buffer by Field_Map182+0x12a
