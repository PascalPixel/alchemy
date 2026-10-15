	.section .rodata
	.global Tileset_Set40TilesD
Tileset_Set40TilesD:
	.incbin "games/tla/src/graphics/tileset/set_40/tiles_d.4bpp.mtf"
	.byte 0xe3, 0xf4	@ PACKER: left in the buffer by Tileset_Set40TilesC+0xf6e
