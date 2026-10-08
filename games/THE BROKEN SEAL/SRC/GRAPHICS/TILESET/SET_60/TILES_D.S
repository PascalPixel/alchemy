	.section .rodata
	.global Tileset_Set60TilesD
Tileset_Set60TilesD:
	.incbin "GRAPHICS/TILESET/SET_60/TILES_D.4bpp.mtf"
	.byte 0x69	@ PACKER: left in the buffer by Tileset_Set60TilesB+0x1907
