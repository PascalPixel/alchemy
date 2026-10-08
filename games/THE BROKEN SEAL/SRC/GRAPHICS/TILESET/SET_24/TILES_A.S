	.section .rodata
	.global Tileset_Set24TilesA
Tileset_Set24TilesA:
	.incbin "GRAPHICS/TILESET/SET_24/TILES_A.4bpp.mtf"
	/* PACKER: left in the packer's buffer by MAP_032/MAP.o at +0x16fb */
	.byte 0x07
