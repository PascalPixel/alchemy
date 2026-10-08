	.section .rodata
	.global Tileset_Set56TilesD
Tileset_Set56TilesD:
	.incbin "graphics/tileset/set_56/tiles_d.4bpp.mtf"
	.byte 0x08, 0x7e	@ PACKER: left in the buffer by Tileset_Set56TilesC+0x99a
