	.section .rodata
	.global Tileset_Set27TilesA
Tileset_Set27TilesA:
	.incbin "GRAPHICS/TILESET/SET_27/TILES_A.4bpp.mtf"
	.byte 0x60, 0xcf, 0x83	@ PACKER: left in the buffer by Tileset_Set23TilesA+0x2d91
