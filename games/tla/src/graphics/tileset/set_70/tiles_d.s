	.section .rodata
	.global Tileset_Set70TilesD
Tileset_Set70TilesD:
	.incbin "games/tla/src/graphics/tileset/set_70/tiles_d.4bpp.mtf"
	.byte 0x67	@ PACKER: left in the buffer by Tileset_Set70TilesC+0x13f
