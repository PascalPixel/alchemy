	.section .rodata
	.global Tileset_Set11TilesB
Tileset_Set11TilesB:
	.incbin "GRAPHICS/TILESET/SET_11/TILES_B.4bpp.mtf"
	.byte 0x50, 0x92, 0x3f	@ PACKER: left in the buffer by Tileset_Set11TilesA+0x909
