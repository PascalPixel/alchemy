	.section .rodata
	.global Tileset_Set32TilesB
Tileset_Set32TilesB:
	.incbin "GRAPHICS/TILESET/SET_32/TILES_B.4bpp.mtf"
	.byte 0x4d, 0x0c, 0x83	@ PACKER: left in the buffer by Tileset_Set32TilesA+0x1d6d
