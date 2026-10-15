	.section .rodata
	.global Tileset_Set112TilesA
Tileset_Set112TilesA:
	.incbin "games/tla/src/graphics/tileset/set_112/tiles_a.4bpp.mtf"
	.byte 0x7d, 0x00, 0x00	@ PACKER: left in the buffer by Field_Map277+0x1e01
