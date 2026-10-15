	.section .rodata
	.global Tileset_Set63TilesD
Tileset_Set63TilesD:
	.incbin "games/tla/src/graphics/tileset/set_63/tiles_d.4bpp.mtf"
	.byte 0x7b, 0x9c, 0xde	@ PACKER: left in the buffer by Tileset_Set63TilesC+0x104d
