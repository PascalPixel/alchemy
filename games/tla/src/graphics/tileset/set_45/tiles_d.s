	.section .rodata
	.global Tileset_Set45TilesD
Tileset_Set45TilesD:
	.incbin "games/tla/src/graphics/tileset/set_45/tiles_d.4bpp.mtf"
	.byte 0x87	@ PACKER: left in the buffer by Tileset_Set45TilesA+0x17d3
