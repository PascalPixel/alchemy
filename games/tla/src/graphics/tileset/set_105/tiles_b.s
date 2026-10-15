	.section .rodata
	.global Tileset_Set105TilesB
Tileset_Set105TilesB:
	.incbin "games/tla/src/graphics/tileset/set_105/tiles_b.4bpp.mtf"
	.byte 0xc4, 0x0d, 0xef	@ PACKER: left in the buffer by Tileset_Set105TilesA+0x2311
