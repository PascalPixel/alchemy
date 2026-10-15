	.section .rodata
	.global Tileset_Set118TilesD
Tileset_Set118TilesD:
	.incbin "games/tla/src/graphics/tileset/set_118/tiles_d.4bpp.mtf"
	.byte 0x3e, 0x7f	@ PACKER: left in the buffer by Tileset_Set118TilesC+0x206
