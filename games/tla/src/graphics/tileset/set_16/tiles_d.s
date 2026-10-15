	.section .rodata
	.global Tileset_Set16TilesD
Tileset_Set16TilesD:
	.incbin "games/tla/src/graphics/tileset/set_16/tiles_d.4bpp.mtf"
	.byte 0x70	@ PACKER: left in the buffer by Tileset_Set16TilesC+0x13f
