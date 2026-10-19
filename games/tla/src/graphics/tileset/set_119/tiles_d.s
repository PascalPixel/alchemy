	.section .rodata
	.global Tileset_Set119TilesD
Tileset_Set119TilesD:
	.incbin "games/tla/src/graphics/tileset/set_119/tiles_d.4bpp.mtf"
	.byte 0xe5, 0x3d	@ PACKER: left in the buffer by Tileset_Set119TilesC+0x842
