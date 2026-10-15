	.section .rodata
	.global Tileset_Set60TilesD
Tileset_Set60TilesD:
	.incbin "games/tla/src/graphics/tileset/set_60/tiles_d.4bpp.mtf"
	.byte 0xa4	@ PACKER: left in the buffer by Tileset_Set60TilesC+0x18db
