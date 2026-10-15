	.section .rodata
	.global Tileset_Set44TilesB
Tileset_Set44TilesB:
	.incbin "games/tla/src/graphics/tileset/set_44/tiles_b.4bpp.mtf"
	.byte 0x9a, 0xa6, 0xe3	@ PACKER: left in the buffer by Tileset_Set44TilesA+0x1ce9
