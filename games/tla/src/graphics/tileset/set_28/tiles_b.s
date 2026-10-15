	.section .rodata
	.global Tileset_Set28TilesB
Tileset_Set28TilesB:
	.incbin "games/tla/src/graphics/tileset/set_28/tiles_b.4bpp.mtf"
	.byte 0xce, 0x80	@ PACKER: left in the buffer by Tileset_Set28TilesA+0x1cbe
