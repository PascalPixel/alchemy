	.section .rodata
	.global Tileset_Set07TilesB
Tileset_Set07TilesB:
	.incbin "games/tla/src/graphics/tileset/set_07/tiles_b.4bpp.mtf"
	.byte 0x58	@ PACKER: left in the buffer by Tileset_Set07TilesA+0x12b3
