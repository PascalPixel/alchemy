	.section .rodata
	.global Tileset_Set60TilesA
Tileset_Set60TilesA:
	.incbin "GRAPHICS/TILESET/SET_60/TILES_A.4bpp.mtf"
	.byte 0x63	@ PACKER: left in the buffer by Tileset_Set59TilesA+0x29df
