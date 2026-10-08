	.section .rodata
	.global Tileset_Set60TilesC
Tileset_Set60TilesC:
	.incbin "graphics/tileset/set_60/tiles_c.4bpp.mtf"
	.byte 0x25, 0x96, 0x85	@ PACKER: left in the buffer by Tileset_Set60TilesB+0x1895
