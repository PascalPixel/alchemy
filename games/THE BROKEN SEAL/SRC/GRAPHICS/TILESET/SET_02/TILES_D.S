	.section .rodata
	.global Tileset_Set02TilesD
Tileset_Set02TilesD:
	.incbin "GRAPHICS/TILESET/SET_02/TILES_D.4bpp.mtf"
	.byte 0xf7	@ PACKER: left in the buffer by Tileset_Set02TilesC+0x1073
