	.section .rodata
	.global Tileset_Set44TilesD
Tileset_Set44TilesD:
	.incbin "games/tla/src/graphics/tileset/set_44/tiles_d.4bpp.mtf"
	.byte 0xf6, 0x81, 0x3a	@ PACKER: left in the buffer by Tileset_Set44TilesC+0x1579
