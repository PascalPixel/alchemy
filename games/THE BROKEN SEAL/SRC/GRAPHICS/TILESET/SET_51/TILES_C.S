	.section .rodata
	.global Tileset_Set51TilesC
Tileset_Set51TilesC:
	.incbin "GRAPHICS/TILESET/SET_51/TILES_C.4bpp.mtf"
	.byte 0x24, 0xda, 0xbb	@ PACKER: left in the buffer by Tileset_Set51TilesB+0x1865
