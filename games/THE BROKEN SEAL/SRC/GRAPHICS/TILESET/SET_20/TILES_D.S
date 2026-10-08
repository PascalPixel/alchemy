	.section .rodata
	.global Tileset_Set20TilesD
Tileset_Set20TilesD:
	.incbin "GRAPHICS/TILESET/SET_20/TILES_D.4bpp.mtf"
	.byte 0xc6, 0x39	@ PACKER: left in the buffer by Tileset_Set20TilesB+0x1a36
