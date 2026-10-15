	.section .rodata
	.global Tileset_Set89TilesB
Tileset_Set89TilesB:
	.incbin "games/tla/src/graphics/tileset/set_89/tiles_b.4bpp.mtf"
	.byte 0x04, 0x7a	@ PACKER: left in the buffer by Tileset_Set89TilesA+0x199a
