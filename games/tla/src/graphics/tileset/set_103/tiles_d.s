	.section .rodata
	.global Tileset_Set103TilesD
Tileset_Set103TilesD:
	.incbin "games/tla/src/graphics/tileset/set_103/tiles_d.4bpp.mtf"
	.byte 0x44, 0x16	@ PACKER: left in the buffer by Tileset_Set103TilesC+0xac2
