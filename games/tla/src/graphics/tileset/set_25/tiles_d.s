	.section .rodata
	.global Tileset_Set25TilesD
Tileset_Set25TilesD:
	.incbin "games/tla/src/graphics/tileset/set_25/tiles_d.4bpp.mtf"
	.byte 0x05	@ PACKER: left in the buffer by Tileset_Set25TilesC+0x13f
