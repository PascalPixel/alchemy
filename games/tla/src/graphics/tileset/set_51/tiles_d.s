	.section .rodata
	.global Tileset_Set51TilesD
Tileset_Set51TilesD:
	.incbin "games/tla/src/graphics/tileset/set_51/tiles_d.4bpp.mtf"
	.byte 0xe0, 0x75	@ PACKER: left in the buffer by Tileset_Set51TilesC+0x30a
