	.section .rodata
	.global Tileset_Set11TilesC
Tileset_Set11TilesC:
	.incbin "graphics/tileset/set_11/tiles_c.4bpp.mtf"
	.byte 0xe1	@ PACKER: left in the buffer by Tileset_Set11TilesB+0x147
