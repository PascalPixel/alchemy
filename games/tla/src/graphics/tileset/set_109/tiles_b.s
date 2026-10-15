	.section .rodata
	.global Tileset_Set109TilesB
Tileset_Set109TilesB:
	.incbin "games/tla/src/graphics/tileset/set_109/tiles_b.4bpp.mtf"
	.byte 0xa6, 0x68, 0x02	@ PACKER: left in the buffer by Tileset_Set109TilesA+0x21c5
