	.section .rodata
	.global Tileset_Set98TilesC
Tileset_Set98TilesC:
	.incbin "games/tla/src/graphics/tileset/set_98/tiles_c.4bpp.mtf"
	.byte 0x3a, 0x6c, 0xb7	@ PACKER: left in the buffer by Tileset_Set98TilesA+0x1a01
