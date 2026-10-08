	.section .rodata
	.global Tileset_Set16TilesC
Tileset_Set16TilesC:
	.incbin "GRAPHICS/TILESET/SET_16/TILES_C.4bpp.mtf"
	.byte 0x1d, 0x65	@ PACKER: left in the buffer by Tileset_Set16TilesB+0x186
