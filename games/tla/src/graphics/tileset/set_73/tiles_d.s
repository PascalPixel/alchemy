	.section .rodata
	.global Tileset_Set73TilesD
Tileset_Set73TilesD:
	.incbin "games/tla/src/graphics/tileset/set_73/tiles_d.4bpp.mtf"
	.byte 0x10	@ PACKER: left in the buffer by Tileset_Set73TilesB+0x27d3
