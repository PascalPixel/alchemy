	.section .rodata
	.global Tileset_Set79TilesD
Tileset_Set79TilesD:
	.incbin "games/tla/src/graphics/tileset/set_79/tiles_d.4bpp.mtf"
	.byte 0x11	@ PACKER: left in the buffer by Tileset_Set79TilesC+0x13f
