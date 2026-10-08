	.section .rodata
	.global Tileset_Set61TilesD
Tileset_Set61TilesD:
	.incbin "graphics/tileset/set_61/tiles_d.4bpp.mtf"
	.byte 0x7f, 0x3d, 0x47	@ PACKER: left in the buffer by Tileset_Set61TilesA+0x1e7d
