	.section .rodata
	.global Tileset_Set42TilesA
Tileset_Set42TilesA:
	.incbin "GRAPHICS/TILESET/SET_42/TILES_A.4bpp.mtf"
	.byte 0x47, 0x90, 0x0e	@ PACKER: left in the buffer by Tileset_Set40TilesA+0x2901
