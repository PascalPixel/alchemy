	.section .rodata
	.global Tileset_Set20TilesD
Tileset_Set20TilesD:
	.incbin "games/tla/src/graphics/tileset/set_20/tiles_d.4bpp.mtf"
	.byte 0x1f, 0xf8	@ PACKER: left in the buffer by Tileset_Set20TilesC+0x1be
