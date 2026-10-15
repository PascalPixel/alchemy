	.section .rodata
	.global Tileset_Set64TilesA
Tileset_Set64TilesA:
	.incbin "games/tla/src/graphics/tileset/set_64/tiles_a.4bpp.mtf"
	.byte 0x00, 0x1b, 0x22	@ PACKER: left in the buffer by Field_Map131+0x2091
