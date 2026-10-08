	.section .rodata
	.global Tileset_Set07TilesD
Tileset_Set07TilesD:
	.incbin "common/graphics/tileset/set_07/tiles_d.4bpp.mtf"
	.byte 0x4e	@ PACKER: left in the buffer by Tileset_Set07TilesC+0x13f
