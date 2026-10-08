	.section .rodata
	.global Tileset_Set70TilesD
Tileset_Set70TilesD:
	.incbin "GRAPHICS/TILESET/SET_68/TILES_B.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set70TilesA+0x13f
