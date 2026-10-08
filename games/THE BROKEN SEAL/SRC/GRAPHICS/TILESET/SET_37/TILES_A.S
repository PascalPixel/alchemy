	.section .rodata
	.global Tileset_Set37TilesA
Tileset_Set37TilesA:
	.incbin "GRAPHICS/TILESET/SET_37/TILES_A.4bpp.mtf"
	.byte 0xe0, 0x91, 0x0f	@ PACKER: left in the buffer by Tileset_Set36TilesB+0x1d59
