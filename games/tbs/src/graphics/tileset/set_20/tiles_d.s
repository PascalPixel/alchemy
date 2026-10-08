	.section .rodata
	.global Tileset_Set20TilesD
Tileset_Set20TilesD:
	.incbin "graphics/tileset/set_20/tiles_d.4bpp.mtf"
	.byte 0xc6, 0x39	@ PACKER: left in the buffer by Tileset_Set20TilesB+0x1a36
