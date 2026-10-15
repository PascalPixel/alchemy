	.section .rodata
	.global Tileset_Set95Palette
Tileset_Set95Palette:
	.incbin "games/tla/src/graphics/tileset/set_95/palette.gbapal.lz"
	.byte 0x80, 0x07, 0x98	@ PACKER: left in the buffer by Field_Map221+0x1d5
