	.section .rodata
	.global Tileset_Set01TilesD
Tileset_Set01TilesD:
	.incbin "GRAPHICS/TILESET/SET_01/TILES_D.4bpp.mtf"
	.byte 0x8f	@ PACKER: left in the buffer by Tileset_Set01TilesC+0x1ecb
