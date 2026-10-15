	.section .rodata
	.global Tileset_Set03TilesD
Tileset_Set03TilesD:
	.incbin "games/tla/src/graphics/tileset/set_03/tiles_d.4bpp.mtf"
	.byte 0x82, 0x65	@ PACKER: left in the buffer by Tileset_Set03TilesC+0x1b3a
