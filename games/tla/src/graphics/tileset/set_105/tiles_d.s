	.section .rodata
	.global Tileset_Set105TilesD
Tileset_Set105TilesD:
	.incbin "games/tla/src/graphics/tileset/set_105/tiles_d.4bpp.mtf"
	.byte 0xf5	@ PACKER: left in the buffer by Tileset_Set105TilesC+0x13f
