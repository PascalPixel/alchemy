	.section .rodata
	.global Tileset_Set26TilesD
Tileset_Set26TilesD:
	.incbin "games/tla/src/graphics/tileset/set_26/tiles_d.4bpp.mtf"
	.byte 0x05	@ PACKER: left in the buffer by Tileset_Set26TilesC+0x13f
