	.section .rodata
	.global Tileset_Set96TilesD
Tileset_Set96TilesD:
	.incbin "games/tla/src/graphics/tileset/set_96/tiles_d.4bpp.mtf"
	.byte 0x0e	@ PACKER: left in the buffer by Tileset_Set96TilesC+0x13f
