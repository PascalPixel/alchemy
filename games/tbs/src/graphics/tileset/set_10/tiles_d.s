	.section .rodata
	.global Tileset_Set10TilesD
Tileset_Set10TilesD:
	.incbin "graphics/tileset/set_10/tiles_d.4bpp.mtf"
	.byte 0xe3, 0xb0, 0xd3	@ PACKER: left in the buffer by Tileset_Set10TilesC+0x225
