	.section .rodata
	.global Tileset_Set91TilesC
Tileset_Set91TilesC:
	.incbin "games/tla/src/graphics/tileset/set_91/tiles_c.4bpp.mtf"
	.byte 0xde	@ PACKER: left in the buffer by Tileset_Set91TilesA+0x195f
