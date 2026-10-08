	.section .rodata
	.global Tileset_Set48TilesB
Tileset_Set48TilesB:
	.incbin "GRAPHICS/TILESET/SET_48/TILES_B.4bpp.mtf"
	.byte 0xd6, 0x54	@ PACKER: left in the buffer by Tileset_Set48TilesA+0x2062
