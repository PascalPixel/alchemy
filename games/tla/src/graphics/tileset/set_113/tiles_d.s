	.section .rodata
	.global Tileset_Set113TilesD
Tileset_Set113TilesD:
	.incbin "games/tla/src/graphics/tileset/set_113/tiles_d.4bpp.mtf"
	.byte 0x88, 0x5f	@ PACKER: left in the buffer by Tileset_Set113TilesC+0x3fa
