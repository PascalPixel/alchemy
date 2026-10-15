	.section .rodata
	.global Tileset_Set21TilesD
Tileset_Set21TilesD:
	.incbin "games/tla/src/graphics/tileset/set_21/tiles_d.4bpp.mtf"
	.byte 0x0f, 0x7c, 0xfe	@ PACKER: left in the buffer by Tileset_Set21TilesC+0x2b5
