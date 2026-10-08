	.section .rodata
	.global Tileset_Set00TilesD
Tileset_Set00TilesD:
	.incbin "GRAPHICS/TILESET/SET_00/TILES_D.4bpp.mtf"
	.byte 0x14, 0x85, 0xc2	@ PACKER: left in the buffer by Tileset_Set00TilesC+0x1e8d
