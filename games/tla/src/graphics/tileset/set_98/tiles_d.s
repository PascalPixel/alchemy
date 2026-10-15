	.section .rodata
	.global Tileset_Set98TilesD
Tileset_Set98TilesD:
	.incbin "games/tla/src/graphics/tileset/set_98/tiles_d.4bpp.mtf"
	.byte 0x84, 0xb0, 0xa3	@ PACKER: left in the buffer by Tileset_Set98TilesC+0x11b9
