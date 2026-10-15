	.section .rodata
	.global Tileset_Set52TilesB
Tileset_Set52TilesB:
	.incbin "games/tla/src/graphics/tileset/set_52/tiles_b.4bpp.mtf"
	.byte 0xc8, 0xf7	@ PACKER: left in the buffer by Tileset_Set52TilesA+0x1f92
