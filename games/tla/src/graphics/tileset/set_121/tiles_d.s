	.section .rodata
	.global Tileset_Set121TilesD
Tileset_Set121TilesD:
	.incbin "games/common/src/graphics/tileset/set_08/tiles_d.4bpp.mtf"
	.byte 0xe5, 0x3d	@ PACKER: left in the buffer by Tileset_Set121TilesC+0x842
