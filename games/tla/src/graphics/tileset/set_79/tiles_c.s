	.section .rodata
	.global Tileset_Set79TilesC
Tileset_Set79TilesC:
	.incbin "games/tla/src/graphics/tileset/set_79/tiles_c.4bpp.mtf"
	.byte 0x92, 0xe5	@ PACKER: left in the buffer by Tileset_Set79TilesB+0x1e76
