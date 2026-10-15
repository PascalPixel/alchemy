	.section .rodata
	.global Tileset_Set06TilesC
Tileset_Set06TilesC:
	.incbin "games/tla/src/graphics/tileset/set_06/tiles_c.4bpp.mtf"
	.byte 0xc1	@ PACKER: left in the buffer by Tileset_Set06TilesA+0x2943
