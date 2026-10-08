	.section .rodata
	.global Tileset_Set25TilesA
Tileset_Set25TilesA:
	.incbin "GRAPHICS/TILESET/SET_25/TILES_A.4bpp.mtf"
	/* PACKER: left in the packer's buffer by MAP_032/MAP.o at +0x1c57 */
	.byte 0x06
