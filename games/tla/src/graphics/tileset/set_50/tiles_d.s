	.section .rodata
	.global Tileset_Set50TilesD
Tileset_Set50TilesD:
	.incbin "games/tla/src/graphics/tileset/set_50/tiles_d.4bpp.mtf"
	.byte 0x7f, 0xe0	@ PACKER: left in the buffer by Tileset_Set50TilesC+0x1be
