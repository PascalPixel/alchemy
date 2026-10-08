	.section .rodata
	.global Tileset_Set27TilesD
Tileset_Set27TilesD:
	.incbin "GRAPHICS/TILESET/SET_27/TILES_D.4bpp.mtf"
	.byte 0xa9	@ PACKER: left in the buffer by Tileset_Set27TilesC+0x13f
