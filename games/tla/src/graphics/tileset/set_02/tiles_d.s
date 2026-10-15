	.section .rodata
	.global Tileset_Set02TilesD
Tileset_Set02TilesD:
	.incbin "games/tla/src/graphics/tileset/set_02/tiles_d.4bpp.mtf"
	.byte 0x0c	@ PACKER: left in the buffer by Tileset_Set02TilesB+0x1943
