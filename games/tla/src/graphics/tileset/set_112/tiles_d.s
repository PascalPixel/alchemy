	.section .rodata
	.global Tileset_Set112TilesD
Tileset_Set112TilesD:
	.incbin "games/tla/src/graphics/tileset/set_112/tiles_d.4bpp.mtf"
	.byte 0xfb, 0x4f	@ PACKER: left in the buffer by Tileset_Set112TilesB+0x13fa
