	.section .rodata
	.global Tileset_Set01TilesD
Tileset_Set01TilesD:
	.incbin "graphics/tileset/set_01/tiles_d.4bpp.mtf"
	.byte 0x8f	@ PACKER: left in the buffer by Tileset_Set01TilesC+0x1ecb
