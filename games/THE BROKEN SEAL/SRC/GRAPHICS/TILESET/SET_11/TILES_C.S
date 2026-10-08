	.section .rodata
	.global Tileset_Set11TilesC
Tileset_Set11TilesC:
	.incbin "GRAPHICS/TILESET/SET_11/TILES_C.4bpp.mtf"
	.byte 0xe1	@ PACKER: left in the buffer by Tileset_Set11TilesB+0x147
