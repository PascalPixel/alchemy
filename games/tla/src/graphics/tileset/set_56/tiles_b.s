	.section .rodata
	.global Tileset_Set56TilesB
Tileset_Set56TilesB:
	.incbin "games/tla/src/graphics/tileset/set_56/tiles_b.4bpp.mtf"
	.byte 0x27, 0xe0, 0xfb	@ PACKER: left in the buffer by Tileset_Set56TilesA+0xe0d
