	.section .rodata
	.global Tileset_Set18TilesD
Tileset_Set18TilesD:
	.incbin "games/tla/src/graphics/tileset/set_18/tiles_d.4bpp.mtf"
	.byte 0x1f, 0xf8	@ PACKER: left in the buffer by Tileset_Set18TilesC+0x1be
