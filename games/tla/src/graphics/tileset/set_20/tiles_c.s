	.section .rodata
	.global Tileset_Set20TilesC
Tileset_Set20TilesC:
	.incbin "games/tla/src/graphics/tileset/set_20/tiles_c.4bpp.mtf"
	.byte 0x83, 0xb4, 0x9c	@ PACKER: left in the buffer by Tileset_Set20TilesB+0x1f5
