	.section .rodata
	.global Tileset_Set55TilesD
Tileset_Set55TilesD:
	.incbin "games/tla/src/graphics/tileset/set_55/tiles_d.4bpp.mtf"
	.byte 0x20	@ PACKER: left in the buffer by Tileset_Set55TilesC+0x13f
