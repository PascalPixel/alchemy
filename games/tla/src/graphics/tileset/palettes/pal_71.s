	.section .rodata
	.global Tileset_Palette71
Tileset_Palette71:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_71.gbapal.lz"
	.byte 0x68, 0xe9, 0x10	@ PACKER: left in the buffer by Field_Map288+0x151
