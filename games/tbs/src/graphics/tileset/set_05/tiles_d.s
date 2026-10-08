	.section .rodata
	.global Tileset_Set05TilesD
Tileset_Set05TilesD:
	.incbin "graphics/tileset/set_05/tiles_d.4bpp.mtf"
	.byte 0x7e	@ PACKER: left in the buffer by Tileset_Set05TilesC+0x150b
