	.section .rodata
	.global Tileset_Set36TilesD
Tileset_Set36TilesD:
	.incbin "GRAPHICS/TILESET/SET_36/TILES_D.4bpp.mtf"
	.byte 0xa4	@ PACKER: left in the buffer by Tileset_Set36TilesC+0x13f
