	.section .rodata
	.global Tileset_Set36TilesC
Tileset_Set36TilesC:
	.incbin "graphics/tileset/set_36/tiles_c.4bpp.mtf"
	.byte 0xe8, 0x95, 0x5a	@ PACKER: left in the buffer by Tileset_Set36TilesB+0xc81
