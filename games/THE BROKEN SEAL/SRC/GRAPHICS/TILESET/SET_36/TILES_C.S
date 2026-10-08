	.section .rodata
	.global Tileset_Set36TilesC
Tileset_Set36TilesC:
	.incbin "GRAPHICS/TILESET/SET_36/TILES_C.4bpp.mtf"
	.byte 0xe8, 0x95, 0x5a	@ PACKER: left in the buffer by Tileset_Set36TilesB+0xc81
