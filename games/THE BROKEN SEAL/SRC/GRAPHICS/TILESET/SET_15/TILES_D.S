	.section .rodata
	.global Tileset_Set15TilesD
Tileset_Set15TilesD:
	.incbin "GRAPHICS/TILESET/SET_15/TILES_D.4bpp.mtf"
	.byte 0x6f	@ PACKER: left in the buffer by Tileset_Set15TilesC+0xcaf
