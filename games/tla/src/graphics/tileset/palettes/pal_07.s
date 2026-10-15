	.section .rodata
	.global Tileset_Palette07
Tileset_Palette07:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_07.gbapal.lz"
	.byte 0x33, 0xbc, 0x5a	@ PACKER: left in the buffer by Field_Map058+0x13d
