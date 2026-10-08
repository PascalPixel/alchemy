	.section .rodata
	.global Tileset_Set04TilesD
Tileset_Set04TilesD:
	.incbin "GRAPHICS/TILESET/SET_04/TILES_D.4bpp.mtf"
	.byte 0x3c	@ PACKER: left in the buffer by Tileset_Set04TilesC+0x12c7
