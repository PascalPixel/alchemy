	.section .rodata
	.global Tileset_Set71TilesC
Tileset_Set71TilesC:
	.incbin "games/tla/src/graphics/tileset/set_71/tiles_c.4bpp.mtf"
	.byte 0x7c, 0x3c, 0x07	@ PACKER: left in the buffer by Tileset_Set71TilesB+0x118d
