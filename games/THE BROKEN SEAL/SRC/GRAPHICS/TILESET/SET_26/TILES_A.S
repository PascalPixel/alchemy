	.section .rodata
	.global Tileset_Set26TilesA
Tileset_Set26TilesA:
	.incbin "GRAPHICS/TILESET/SET_26/TILES_A.4bpp.mtf"
	.byte 0x9e	@ PACKER: left in the buffer by Tileset_Set23TilesA+0x2b3b
