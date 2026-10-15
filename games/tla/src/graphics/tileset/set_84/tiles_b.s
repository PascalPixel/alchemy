	.section .rodata
	.global Tileset_Set84TilesB
Tileset_Set84TilesB:
	.incbin "games/tla/src/graphics/tileset/set_84/tiles_b.4bpp.mtf"
	.byte 0x40, 0x60, 0xb9	@ PACKER: left in the buffer by Tileset_Set84TilesA+0x13f5
