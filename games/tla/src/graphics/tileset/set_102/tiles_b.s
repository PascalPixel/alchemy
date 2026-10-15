	.section .rodata
	.global Tileset_Set102TilesB
Tileset_Set102TilesB:
	.incbin "games/tla/src/graphics/tileset/set_102/tiles_b.4bpp.mtf"
	.byte 0x46	@ PACKER: left in the buffer by Tileset_Set101TilesA+0x1edb
