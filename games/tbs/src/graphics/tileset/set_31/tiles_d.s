	.section .rodata
	.global Tileset_Set31TilesD
Tileset_Set31TilesD:
	.incbin "graphics/tileset/set_31/tiles_d.4bpp.mtf"
	.byte 0x1f, 0xf8	@ PACKER: left in the buffer by Tileset_Set31TilesC+0xe22
