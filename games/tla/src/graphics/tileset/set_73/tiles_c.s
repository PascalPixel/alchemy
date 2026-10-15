	.section .rodata
	.global Tileset_Set73TilesC
Tileset_Set73TilesC:
	.incbin "games/tla/src/graphics/tileset/set_73/tiles_c.4bpp.mtf"
	.byte 0x91, 0x81, 0x07	@ PACKER: left in the buffer by Tileset_Set73TilesB+0x21a5
