	.section .rodata
	.global Tileset_Set14TilesD
Tileset_Set14TilesD:
	.incbin "graphics/tileset/set_14/tiles_d.4bpp.mtf"
	.byte 0x5e, 0x11, 0x7d	@ PACKER: left in the buffer by Tileset_Set14TilesC+0x1851
