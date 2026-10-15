	.section .rodata
	.global Tileset_Set115TilesD
Tileset_Set115TilesD:
	.incbin "games/tla/src/graphics/tileset/set_115/tiles_d.4bpp.mtf"
	.byte 0x73	@ PACKER: left in the buffer by Tileset_Set115TilesC+0x4e3
