	.section .rodata
	.global Tileset_Set05TilesC
Tileset_Set05TilesC:
	.incbin "GRAPHICS/TILESET/SET_05/TILES_C.4bpp.mtf"
	.byte 0x38, 0xf0, 0x3d	@ PACKER: left in the buffer by Tileset_Set05TilesB+0x1609
