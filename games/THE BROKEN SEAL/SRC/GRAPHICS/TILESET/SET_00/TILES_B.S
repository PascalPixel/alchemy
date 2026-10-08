	.section .rodata
	.global Tileset_Set00TilesB
Tileset_Set00TilesB:
	.incbin "GRAPHICS/TILESET/SET_00/TILES_B.4bpp.mtf"
	.byte 0x5d, 0x04, 0x53	@ PACKER: left in the buffer by Tileset_Set00TilesA+0x2529
