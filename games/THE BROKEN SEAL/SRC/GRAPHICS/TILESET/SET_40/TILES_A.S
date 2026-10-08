	.section .rodata
	.global Tileset_Set40TilesA
Tileset_Set40TilesA:
	.incbin "GRAPHICS/TILESET/SET_40/TILES_A.4bpp.mtf"
	.byte 0x45, 0x58	@ PACKER: left in the buffer by Tileset_Set36TilesB+0x2966
