	.section .rodata
	.global Tileset_Set89TilesD
Tileset_Set89TilesD:
	.incbin "games/tla/src/graphics/tileset/set_89/tiles_d.4bpp.mtf"
	.byte 0x0d	@ PACKER: left in the buffer by Tileset_Set89TilesC+0xb83
