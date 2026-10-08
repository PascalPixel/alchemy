	.section .rodata
	.global Tileset_Set13TilesB
Tileset_Set13TilesB:
	.incbin "GRAPHICS/TILESET/SET_13/TILES_B.4bpp.mtf"
	.byte 0xa5	@ PACKER: left in the buffer by Tileset_Set13TilesA+0x1723
