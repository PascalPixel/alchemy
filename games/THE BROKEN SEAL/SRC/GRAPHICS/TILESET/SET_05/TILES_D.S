	.section .rodata
	.global Tileset_Set05TilesD
Tileset_Set05TilesD:
	.incbin "GRAPHICS/TILESET/SET_05/TILES_D.4bpp.mtf"
	.byte 0x7e	@ PACKER: left in the buffer by Tileset_Set05TilesC+0x150b
