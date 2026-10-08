	.section .rodata
	.global Tileset_Set46TilesD
Tileset_Set46TilesD:
	.incbin "graphics/tileset/set_46/tiles_d.4bpp.mtf"
	.byte 0x0a, 0x07	@ PACKER: left in the buffer by Tileset_Set46TilesB+0xc12
