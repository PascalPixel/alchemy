	.section .rodata
	.global Tileset_Set120TilesD
Tileset_Set120TilesD:
	.incbin "games/tla/src/graphics/tileset/set_120/tiles_d.4bpp.mtf"
	.byte 0xe0	@ PACKER: left in the buffer by Tileset_Set120TilesC+0x13f
