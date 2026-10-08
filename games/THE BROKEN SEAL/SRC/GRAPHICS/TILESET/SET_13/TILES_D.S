	.section .rodata
	.global Tileset_Set13TilesD
Tileset_Set13TilesD:
	.incbin "GRAPHICS/TILESET/SET_13/TILES_D.4bpp.mtf"
	.byte 0x01	@ PACKER: left in the buffer by Tileset_Set13TilesC+0x13f
