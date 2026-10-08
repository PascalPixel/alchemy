	.section .rodata
	.global Tileset_Set12TilesD
Tileset_Set12TilesD:
	.incbin "GRAPHICS/TILESET/SET_12/TILES_D.4bpp.mtf"
	.byte 0x01	@ PACKER: left in the buffer by Tileset_Set12TilesC+0x13f
