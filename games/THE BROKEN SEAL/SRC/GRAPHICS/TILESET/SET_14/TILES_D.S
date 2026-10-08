	.section .rodata
	.global Tileset_Set14TilesD
Tileset_Set14TilesD:
	.incbin "GRAPHICS/TILESET/SET_14/TILES_D.4bpp.mtf"
	.byte 0x5e, 0x11, 0x7d	@ PACKER: left in the buffer by Tileset_Set14TilesC+0x1851
