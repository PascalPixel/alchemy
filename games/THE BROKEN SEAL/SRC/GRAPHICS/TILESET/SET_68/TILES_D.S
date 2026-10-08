	.section .rodata
	.global Tileset_Set68TilesD
Tileset_Set68TilesD:
	.incbin "GRAPHICS/TILESET/SET_68/TILES_B.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set68TilesA+0x13f
