	.section .rodata
	.global Tileset_Set18TilesD
Tileset_Set18TilesD:
	.incbin "GRAPHICS/TILESET/SET_18/TILES_D.4bpp.mtf"
	.byte 0x04	@ PACKER: left in the buffer by Tileset_Set18TilesC+0x13f
