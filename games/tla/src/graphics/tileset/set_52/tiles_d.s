	.section .rodata
	.global Tileset_Set52TilesD
Tileset_Set52TilesD:
	.incbin "games/tla/src/graphics/tileset/set_52/tiles_d.4bpp.mtf"
	.byte 0x05	@ PACKER: left in the buffer by Tileset_Set52TilesC+0x13f
