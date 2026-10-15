	.section .rodata
	.global Tileset_Set11TilesD
Tileset_Set11TilesD:
	.incbin "games/tla/src/graphics/tileset/set_11/tiles_d.4bpp.mtf"
	.byte 0x3d, 0x87, 0xa4	@ PACKER: left in the buffer by Tileset_Set11TilesC+0x4d1
