	.section .rodata
	.global Tileset_Set08TilesD
Tileset_Set08TilesD:
	.incbin "common/graphics/tileset/set_08/tiles_d.4bpp.mtf"
	.byte 0xe1, 0x71	@ PACKER: left in the buffer by Tileset_Set08TilesC+0x842
