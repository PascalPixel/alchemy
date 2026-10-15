	.section .rodata
	.global Tileset_Set119TilesD
Tileset_Set119TilesD:
	.incbin "games/common/src/graphics/tileset/set_08/tiles_d.4bpp.mtf"
	.byte 0xe5, 0x3d	@ PACKER: left in the buffer by Tileset_Set119TilesC+0x842
