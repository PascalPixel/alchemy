	.section .rodata
	.global Tileset_Set21TilesD
Tileset_Set21TilesD:
	.incbin "GRAPHICS/TILESET/SET_21/TILES_D.4bpp.mtf"
	.byte 0x7c, 0x2f	@ PACKER: left in the buffer by Tileset_Set21TilesC+0x1cea
