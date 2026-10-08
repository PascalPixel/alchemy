	.section .rodata
	.global Tileset_Set00TilesD
Tileset_Set00TilesD:
	.incbin "graphics/tileset/set_00/tiles_d.4bpp.mtf"
	.byte 0x14, 0x85, 0xc2	@ PACKER: left in the buffer by Tileset_Set00TilesC+0x1e8d
