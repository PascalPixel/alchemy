	.section .rodata
	.global Tileset_Set112TilesB
Tileset_Set112TilesB:
	.incbin "games/tla/src/graphics/tileset/set_112/tiles_b.4bpp.mtf"
	.byte 0x00, 0x07	@ PACKER: left in the buffer by Tileset_Set112TilesA+0x1ac2
