	.section .rodata
	.global Tileset_Set36TilesD
Tileset_Set36TilesD:
	.incbin "graphics/tileset/set_36/tiles_d.4bpp.mtf"
	.byte 0xa4	@ PACKER: left in the buffer by Tileset_Set36TilesC+0x13f
