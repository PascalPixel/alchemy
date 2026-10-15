	.section .rodata
	.global Tileset_Set55TilesA
Tileset_Set55TilesA:
	.incbin "games/tla/src/graphics/tileset/set_55/tiles_a.4bpp.mtf"
	.byte 0x22, 0xe3, 0x07	@ PACKER: left in the buffer by Field_Map106+0x1949
