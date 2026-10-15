	.section .rodata
	.global Tileset_Set44TilesC
Tileset_Set44TilesC:
	.incbin "games/tla/src/graphics/tileset/set_44/tiles_c.4bpp.mtf"
	.byte 0x88, 0xf0, 0x05	@ PACKER: left in the buffer by Tileset_Set44TilesB+0x16fd
