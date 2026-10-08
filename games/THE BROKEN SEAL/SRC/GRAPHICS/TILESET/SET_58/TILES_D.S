	.section .rodata
	.global Tileset_Set58TilesD
Tileset_Set58TilesD:
	.incbin "GRAPHICS/TILESET/SET_58/TILES_D.4bpp.mtf"
	.byte 0xff	@ PACKER: left in the buffer by Tileset_Set58TilesC+0x13f
