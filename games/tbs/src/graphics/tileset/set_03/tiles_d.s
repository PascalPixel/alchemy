	.section .rodata
	.global Tileset_Set03TilesD
Tileset_Set03TilesD:
	.incbin "graphics/tileset/set_03/tiles_d.4bpp.mtf"
	.byte 0xe1, 0xf0, 0xf8	@ PACKER: left in the buffer by Tileset_Set03TilesC+0x1145
