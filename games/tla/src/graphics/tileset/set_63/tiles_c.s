	.section .rodata
	.global Tileset_Set63TilesC
Tileset_Set63TilesC:
	.incbin "games/tla/src/graphics/tileset/set_63/tiles_c.4bpp.mtf"
	.byte 0xbb, 0x1f	@ PACKER: left in the buffer by Tileset_Set63TilesB+0x1482
