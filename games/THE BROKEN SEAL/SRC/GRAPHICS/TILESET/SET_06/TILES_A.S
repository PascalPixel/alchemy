	.section .rodata
	.global Tileset_Set06TilesA
Tileset_Set06TilesA:
	.incbin "GRAPHICS/TILESET/SET_06/TILES_A.4bpp.mtf"
	.byte 0x8f, 0x85	@ PACKER: left in the buffer by Tileset_Set05TilesD+0x13e2
